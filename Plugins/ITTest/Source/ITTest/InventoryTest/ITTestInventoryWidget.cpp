// Fill out your copyright notice in the Description page of Project Settings.


#include "ITTestInventoryWidget.h"
#include "ITTestInventoryComponent.h"
#include "ITTestInventorySlotWidget.h"
#include "Components/GridPanel.h"

void UITTestInventoryWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (InventoryComponent)
	{
		InventoryComponent->OnInventoryUpdated.AddDynamic(this, &UITTestInventoryWidget::RebuildInventoryGrid);
	}

	RebuildInventoryGrid();
}

void UITTestInventoryWidget::RebuildInventoryGrid()
{
	if (!InventoryComponent || !ItemGrid || !InventorySlotClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("Inventory Grid Rebuild Failed"));
		return;
	}

	ItemGrid->ClearChildren();

	const TArray<UITTestItemInstance*>& Items = InventoryComponent->GetItems();

	for (int32 i = 0; i < Items.Num(); i++)
	{
		UITTestItemInstance* CurrentItem = Items[i];
		UITTestInventorySlotWidget* SlotWidget
			= CreateWidget<UITTestInventorySlotWidget>(this, InventorySlotClass);
		if (SlotWidget)
		{
			SlotWidget->InitializeSlot(CurrentItem, i);

			const int32 Row = i % GridRowSize;
			const int32 Column = i / GridRowSize;
			ItemGrid->AddChildToGrid(SlotWidget, Row, Column);
		}
	}
}
