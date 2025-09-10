// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ITTestInventoryWidget.generated.h"

class UGridPanel;
class UITTestInventorySlotWidget;
class UITTestInventoryComponent;

UCLASS()
class ITTEST_API UITTestInventoryWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Inventory", meta = (ExposeOnSpawn = "true"))
	TObjectPtr<UITTestInventoryComponent> InventoryComponent;

protected:
	virtual void NativeConstruct() override;

	UFUNCTION()
	void RebuildInventoryGrid();

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UGridPanel> ItemGrid;

	UPROPERTY(EditDefaultsOnly, Category = "Inventory")
	TSubclassOf<UITTestInventorySlotWidget> InventorySlotClass;

	UPROPERTY(EditDefaultsOnly, Category = "Inventory")
	int32 GridRowSize = 2;
};
