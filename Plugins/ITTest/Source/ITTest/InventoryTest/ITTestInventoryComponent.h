// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ITTestInventoryComponent.generated.h"

class UITTestItemData;
class UITTestItemInstance;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInventoryUpdated);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ITTEST_API UITTestInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UITTestInventoryComponent();

	UFUNCTION(BlueprintPure, Category="Inventory")
	const TArray<UITTestItemInstance*>& GetItems() const;

	UFUNCTION(BlueprintCallable, Server, Reliable, Category = "Inventory")
	void Server_AddItem(UITTestItemData* ItemData, int32 AddQuantity);

	UFUNCTION(BlueprintCallable, Server, Reliable, Category = "Inventory")
	void Server_RemoveItem(int32 SlotIndex);

	UPROPERTY(BlueprintAssignable, Category = "Inventory|Events")
	FOnInventoryUpdated OnInventoryUpdated;

protected:
	virtual void BeginPlay() override;
	virtual bool ReplicateSubobjects(
		class UActorChannel* Channel, class FOutBunch* Bunch, FReplicationFlags* RepFlags) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION()
	void OnRep_InventoryItems();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Inventory")
	int32 InventorySize = 8;

	UPROPERTY(ReplicatedUsing = OnRep_InventoryItems)
	TArray<TObjectPtr<UITTestItemInstance>> InventoryItems;
};
