// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ITTestInventorySlotWidget.generated.h"

class UTextBlock;
class UImage;
class UITTestItemInstance;

UCLASS()
class ITTEST_API UITTestInventorySlotWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void InitializeSlot(UITTestItemInstance* InItemInstance, int32 InSlotIndex);

protected:
	UFUNCTION()
	void UpdateSlot();

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> ItemIconImage;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> QuantityText;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Inventory Slot")
	TObjectPtr<UITTestItemInstance> ItemInstance;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Inventory Slot")
	int32 SlotIndex;
};
