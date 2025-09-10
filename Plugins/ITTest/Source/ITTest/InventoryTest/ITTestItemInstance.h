// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ITTestItemInstance.generated.h"

class UITTestItemData;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnItemInstanceUpdated);

UCLASS(BlueprintType)
class ITTEST_API UITTestItemInstance : public UObject
{
	GENERATED_BODY()

public:
	virtual bool IsSupportedForNetworking() const override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	void InitItemInstance(UITTestItemData* InitItemData, int32 InitQuantity = 1);

	UPROPERTY(BlueprintAssignable, Category = "Item|Events")
	FOnItemInstanceUpdated OnItemInstanceUpdated;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Replicated, Category = "Item")
	TObjectPtr<UITTestItemData> ItemData;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, ReplicatedUsing = OnRep_Quantity, Category = "Item")
	int32 Quantity;

protected:
	UFUNCTION()
	void OnRep_Quantity();
};
