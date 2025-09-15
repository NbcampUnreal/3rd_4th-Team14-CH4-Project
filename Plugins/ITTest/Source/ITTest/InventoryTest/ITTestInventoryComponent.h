// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ITTestInventoryComponent.generated.h"

class AITTestItemActor;
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
	void Server_AddPickupItem(AITTestItemActor* ItemActorToPickup);

	UFUNCTION(BlueprintCallable, Server, Reliable, Category = "Inventory")
	void Server_RemoveItem(int32 SlotIndex);

	UFUNCTION(BlueprintCallable, Server, Reliable, Category = "Inventory")
	void Server_DropItem(int32 SlotIndex);

	UFUNCTION(BlueprintCallable, Server, Reliable, Category = "Inventory")
	void Server_UseItem(UITTestItemInstance* ItemInstance);

	UFUNCTION(BlueprintCallable, Server, Reliable, Category = "Inventory")
	void Server_ConsumeItem(UITTestItemInstance* ItemInstance, int Quantity = 1);

	// TODO: Prototyping 함수, 퀵 슬롯 제작 후 이전
	UFUNCTION(BlueprintCallable, Server, Reliable, Category = "Inventory")
	void Server_TestUseItem(int32 SlotIndex);
	// 기존의 아이템 추가 함수 -> 테스트용 / 아이템 즉시 획득
	UFUNCTION(BlueprintCallable, Server, Reliable, Category = "Inventory")
	void Server_TestAddItem(UITTestItemData* ItemData, int32 AddQuantity);

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
	int32 InventorySize = 4;

	UPROPERTY(ReplicatedUsing = OnRep_InventoryItems)
	TArray<TObjectPtr<UITTestItemInstance>> InventoryItems;

private:
	// 리플리케이션 함수는 반환값을 가질 수 없으므로, 헬퍼함수 추가
	int32 TryAddItem(UITTestItemData* ItemData, int32 AddQuantity);
};
