// Fill out your copyright notice in the Description page of Project Settings.


#include "ITTestInventorySlotWidget.h"

#include "ITTestItemData.h"
#include "ITTestItemInstance.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Engine/AssetManager.h"

void UITTestInventorySlotWidget::InitializeSlot(UITTestItemInstance* InItemInstance, int32 InSlotIndex)
{
	ItemInstance = InItemInstance;
	SlotIndex = InSlotIndex;

	if (ItemInstance)
	{
		ItemInstance->OnItemInstanceUpdated.AddDynamic(this, &UITTestInventorySlotWidget::UpdateSlot);
	}

	UpdateSlot();
}

void UITTestInventorySlotWidget::UpdateSlot()
{
	if (!ItemInstance || !ItemInstance->ItemData)
	{
		ItemIconImage->SetVisibility(ESlateVisibility::Hidden);
		QuantityText->SetVisibility(ESlateVisibility::Hidden);
		return;
	}

	if (ItemIconImage)
	{
		ItemIconImage->SetVisibility(ESlateVisibility::Visible);
		FStreamableManager& Streamable = UAssetManager::GetStreamableManager();
		Streamable.RequestAsyncLoad(ItemInstance->ItemData->ItemIcon.ToSoftObjectPath(), [this]()
		{
			if (ItemInstance && ItemInstance->ItemData && ItemIconImage)
			{
				ItemIconImage->SetBrushFromTexture(ItemInstance->ItemData->ItemIcon.Get());
			}
		});
	}

	if (QuantityText)
	{
		if (ItemInstance->Quantity > 1)
		{
			QuantityText->SetVisibility(ESlateVisibility::Visible);
			QuantityText->SetText(FText::AsNumber(ItemInstance->Quantity));
		}
		else
		{
			QuantityText->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}
