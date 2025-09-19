// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ITItemActor.generated.h"

class UITItemDefinition;
class USphereComponent;
class UITItemInstance;

UCLASS()
class IMITATIONTRIGGER_API AITItemActor : public AActor
{
	GENERATED_BODY()

public:
	AITItemActor();

	UFUNCTION(BlueprintCallable, Category = "IT|Item")
	UITItemInstance* GetItemInstance() const;

	void InitItemActor(UITItemInstance* InitItemInstance);

protected:
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnRep_ItemInstance();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "IT|Component")
	TObjectPtr<USphereComponent> SphereComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "IT|Component")
	TObjectPtr<UStaticMeshComponent> StaticMeshComponent;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_ItemInstance, Category = "IT")
	TObjectPtr<UITItemInstance> ItemInstance;

	// 레벨 배치 변수
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "IT|Placed")
	TObjectPtr<UITItemDefinition> PlacedItemDefinition;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "IT|Placed", meta = (ClampMin = 1))
	int32 PlacedItemQuantity = 1;

private:
	void UpdateAppearance();
};
