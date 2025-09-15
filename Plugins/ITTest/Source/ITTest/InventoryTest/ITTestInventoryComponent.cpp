// Fill out your copyright notice in the Description page of Project Settings.


#include "ITTestInventoryComponent.h"

#include "AbilitySystemComponent.h"
#include "ITTestItemActor.h"
#include "ITTestItemData.h"
#include "ITTestItemData_Usable.h"
#include "ITTestItemInstance.h"
#include "Abilities/GameplayAbility.h"
#include "Engine/ActorChannel.h"
#include "Net/UnrealNetwork.h"

UITTestInventoryComponent::UITTestInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	SetIsReplicatedByDefault(true);
}

void UITTestInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	if (GetOwner()->HasAuthority())
	{
		InventoryItems.SetNum(InventorySize);
	}
}

bool UITTestInventoryComponent::ReplicateSubobjects(
	class UActorChannel* Channel, class FOutBunch* Bunch, FReplicationFlags* RepFlags)
{
	bool bWroteSomething = Super::ReplicateSubobjects(Channel, Bunch, RepFlags);

	for (UITTestItemInstance* ItemInstance : InventoryItems)
	{
		if (IsValid(ItemInstance))
		{
			bWroteSomething |= Channel->ReplicateSubobject(ItemInstance, *Bunch, *RepFlags);
		}
	}

	return bWroteSomething;
}

void UITTestInventoryComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UITTestInventoryComponent, InventoryItems);
}

void UITTestInventoryComponent::OnRep_InventoryItems()
{
	OnInventoryUpdated.Broadcast();
}

const TArray<UITTestItemInstance*>& UITTestInventoryComponent::GetItems() const
{
	return InventoryItems;
}

int32 UITTestInventoryComponent::TryAddItem(UITTestItemData* ItemData, int32 AddQuantity)
{
	if (ItemData == nullptr || AddQuantity <= 0)
	{
		return 0;
	}

	int32 InitialQuantity = AddQuantity;
	int32 RemainingQuantity = AddQuantity;

	if (ItemData->bIsStackable)
	{
		for (UITTestItemInstance* Item : InventoryItems)
		{
			if (Item && Item->ItemData == ItemData && Item->Quantity < ItemData->MaxStackSize)
			{
				const int32 SpaceInStack = ItemData->MaxStackSize - Item->Quantity;
				const int32 AmountToStack = FMath::Min(RemainingQuantity, SpaceInStack);

				Item->Quantity += AmountToStack;
				RemainingQuantity -= AmountToStack;

				if (RemainingQuantity <= 0)
				{
					break;
				}
			}
		}
	}

	if (RemainingQuantity > 0)
	{
		for (int32 i = 0; i < InventoryItems.Num(); i++)
		{
			if (InventoryItems[i] == nullptr)
			{
				const int32 AmountToPlace = FMath::Min(RemainingQuantity, ItemData->MaxStackSize);

				UITTestItemInstance* NewItemInstance = NewObject<UITTestItemInstance>(GetOwner());
				NewItemInstance->InitItemInstance(ItemData, AmountToPlace);
				InventoryItems[i] = NewItemInstance;

				RemainingQuantity -= AmountToPlace;

				if (RemainingQuantity <= 0)
				{
					break;
				}
			}
		}
	}

	int32 ActuallyAddedQuantity = InitialQuantity - RemainingQuantity;

	if (ActuallyAddedQuantity > 0)
	{
		OnRep_InventoryItems();
	}

	return ActuallyAddedQuantity;
}

void UITTestInventoryComponent::Server_AddPickupItem_Implementation(AITTestItemActor* ItemActorToPickup)
{
	if (!IsValid(ItemActorToPickup) || !IsValid(ItemActorToPickup->GetItemInstance()))
	{
		return;
	}

	UITTestItemInstance* InstanceOnGround = ItemActorToPickup->GetItemInstance();
	UITTestItemData* DataToPickup = InstanceOnGround->ItemData;
	const int32 QuantityToPickup = InstanceOnGround->Quantity;

	const int32 AddedQuantity = TryAddItem(DataToPickup, QuantityToPickup);

	if (AddedQuantity >= QuantityToPickup)
	{
		ItemActorToPickup->Destroy();
	}
	else if (AddedQuantity > 0)
	{
		InstanceOnGround->Quantity -= AddedQuantity;
	}
}

void UITTestInventoryComponent::Server_RemoveItem_Implementation(int32 SlotIndex)
{
	if (!InventoryItems.IsValidIndex(SlotIndex) || InventoryItems[SlotIndex] == nullptr)
	{
		return;
	}

	InventoryItems[SlotIndex] = nullptr;
}

void UITTestInventoryComponent::Server_DropItem_Implementation(int32 SlotIndex)
{
	if (!InventoryItems.IsValidIndex(SlotIndex) || InventoryItems[SlotIndex] == nullptr)
	{
		return;
	}

	UITTestItemInstance* ItemToDrop = InventoryItems[SlotIndex];

	if (GetOwner() && GetWorld())
	{
		FVector DropLocation = GetOwner()->GetActorLocation() + GetOwner()->GetActorUpVector() * 2000.0f;
		FRotator DropRotation = FRotator::ZeroRotator;
		FActorSpawnParameters SpawnParameters;
		SpawnParameters.Owner = GetOwner();
		SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		AITTestItemActor* DroppedItemActor = GetWorld()->SpawnActor<AITTestItemActor>(
			AITTestItemActor::StaticClass(), DropLocation, DropRotation, SpawnParameters);

		if (DroppedItemActor)
		{
			DroppedItemActor->InitializeItem(ItemToDrop);
			Server_RemoveItem(SlotIndex);
		}
	}
}

void UITTestInventoryComponent::Server_UseItem_Implementation(UITTestItemInstance* ItemInstance)
{
	if (!ItemInstance || !ItemInstance->ItemData || !InventoryItems.Contains(ItemInstance))
	{
		return;
	}

	const UITTestItemData_Usable* UsableItemData = Cast<UITTestItemData_Usable>(ItemInstance->ItemData);
	if (!UsableItemData || !UsableItemData->ItemAbility)
	{
		return;
	}

	UAbilitySystemComponent* ASC = GetOwner()->FindComponentByClass<UAbilitySystemComponent>();
	if (!ASC)
	{
		return;
	}

	FGameplayAbilitySpec Spec(UsableItemData->ItemAbility);
	Spec.SourceObject = ItemInstance;

	ASC->GiveAbilityAndActivateOnce(Spec);
}

void UITTestInventoryComponent::Server_ConsumeItem_Implementation(UITTestItemInstance* ItemInstance, int Quantity)
{
	if (!ItemInstance || !InventoryItems.Contains(ItemInstance))
	{
		return;
	}

	int32 ItemIndex;
	InventoryItems.Find(ItemInstance, ItemIndex);

	if (ItemInstance->ItemData->bIsStackable)
	{
		ItemInstance->Quantity -= Quantity;
		if (ItemInstance->Quantity <= 0)
		{
			InventoryItems[ItemIndex] = nullptr;
		}
	}
	else
	{
		InventoryItems[ItemIndex] = nullptr;
	}

	OnRep_InventoryItems();
}

void UITTestInventoryComponent::Server_TestAddItem_Implementation(UITTestItemData* ItemData, int32 AddQuantity)
{
	TryAddItem(ItemData, AddQuantity);
}

void UITTestInventoryComponent::Server_TestUseItem_Implementation(int32 SlotIndex)
{
	if (!InventoryItems.IsValidIndex(SlotIndex) && InventoryItems[SlotIndex] != nullptr)
	{
		return;
	}

	UITTestItemInstance* ItemToUse = InventoryItems[SlotIndex];

	Server_UseItem(ItemToUse);
}
