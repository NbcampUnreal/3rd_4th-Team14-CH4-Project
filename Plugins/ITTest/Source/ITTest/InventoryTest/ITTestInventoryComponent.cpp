// Fill out your copyright notice in the Description page of Project Settings.


#include "ITTestInventoryComponent.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
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
	DOREPLIFETIME(UITTestInventoryComponent, InventorySize);
}

bool UITTestInventoryComponent::HasAuthorityOwner()
{
	return GetOwner() && GetOwner()->HasAuthority();
}

void UITTestInventoryComponent::OnRep_InventoryUpdate()
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
		OnRep_InventoryUpdate();
	}

	return ActuallyAddedQuantity;
}

void UITTestInventoryComponent::ServerRPC_AddPickupItem_Implementation(AITTestItemActor* ItemActorToPickup)
{
	if (HasAuthorityOwner())
	{
		if (!IsValid(ItemActorToPickup) || !IsValid(ItemActorToPickup->GetItemInstance()))
		{
			return;
		}

		if (UITTestItemInstance* InstanceOnGround = ItemActorToPickup->GetItemInstance())
		{
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
	}
}

void UITTestInventoryComponent::ServerRPC_RemoveItem_Implementation(int32 SlotIndex)
{
	if (HasAuthorityOwner())
	{
		if (!InventoryItems.IsValidIndex(SlotIndex) || InventoryItems[SlotIndex] == nullptr)
		{
			return;
		}

		InventoryItems[SlotIndex] = nullptr;
	}
}

void UITTestInventoryComponent::ServerRPC_DropItem_Implementation(int32 SlotIndex)
{
	if (HasAuthorityOwner())
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
			SpawnParameters.SpawnCollisionHandlingOverride
				= ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

			AITTestItemActor* DroppedItemActor = GetWorld()->SpawnActor<AITTestItemActor>(
				AITTestItemActor::StaticClass(), DropLocation, DropRotation, SpawnParameters);

			if (DroppedItemActor)
			{
				DroppedItemActor->InitializeItem(ItemToDrop);
				ServerRPC_RemoveItem(SlotIndex);
			}
		}
	}
}

void UITTestInventoryComponent::ServerRPC_UseItem_Implementation(UITTestItemInstance* ItemInstance)
{
	if (HasAuthorityOwner())
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

		if (IAbilitySystemInterface* ASI = Cast<IAbilitySystemInterface>(GetOwner()))
		{
			if (UAbilitySystemComponent* ASC = ASI->GetAbilitySystemComponent())
			{
				FGameplayAbilitySpec Spec(UsableItemData->ItemAbility);
				Spec.SourceObject = ItemInstance;

				ASC->GiveAbilityAndActivateOnce(Spec);
			}
		}
	}
}

void UITTestInventoryComponent::ServerRPC_ReduceItem_Implementation(UITTestItemInstance* ItemInstance, int Quantity)
{
	if (HasAuthorityOwner())
	{
		if (!ItemInstance)
		{
			return;
		}

		int32 ItemIndex;
		if (!InventoryItems.Find(ItemInstance, ItemIndex))
		{
			return;
		}

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

		OnRep_InventoryUpdate();
	}
}

void UITTestInventoryComponent::ServerRPC_TestAddItem_Implementation(UITTestItemData* ItemData, int32 AddQuantity)
{
	if (HasAuthorityOwner())
	{
		TryAddItem(ItemData, AddQuantity);
	}
}

void UITTestInventoryComponent::ServerRPC_TestUseItem_Implementation(int32 SlotIndex)
{
	if (HasAuthorityOwner())
	{
		if (!InventoryItems.IsValidIndex(SlotIndex) && InventoryItems[SlotIndex] != nullptr)
		{
			return;
		}

		UITTestItemInstance* ItemToUse = InventoryItems[SlotIndex];

		ServerRPC_UseItem(ItemToUse);
	}
}
