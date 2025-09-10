// Fill out your copyright notice in the Description page of Project Settings.


#include "ITTestInventoryComponent.h"
#include "ITTestItemData.h"
#include "ITTestItemInstance.h"
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

void UITTestInventoryComponent::Server_AddItem_Implementation(UITTestItemData* ItemData, int32 AddQuantity)
{
	if (ItemData == nullptr || AddQuantity <= 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("AddItem() return1"));
		return;
	}

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
					return;
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
					return;
				}
			}
		}
	}
}

void UITTestInventoryComponent::Server_RemoveItem_Implementation(int32 SlotIndex)
{
	if (!InventoryItems.IsValidIndex(SlotIndex))
	{
		UE_LOG(LogTemp, Warning, TEXT("RemoveItem() return"));
		return;
	}

	InventoryItems[SlotIndex] = nullptr;
}
