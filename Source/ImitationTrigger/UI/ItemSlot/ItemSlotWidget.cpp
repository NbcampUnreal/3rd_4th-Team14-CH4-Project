// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ItemSlot/ItemSlotWidget.h"

#include "Components/Image.h"
#include "Components/TextBlock.h"

void UItemSlotWidget::UpdateItemAmount(int32 ItemAmount)
{
	if (ItemAmount <= 0)
	{
		ItemAmount = 0;
		ItemImage->SetRenderOpacity(0.5f);
	}
	else
	{
		ItemImage->SetRenderOpacity(1.0f);
	}
	
	
	if (ItemAmountTextBlock)
	{
		FText ItemAmountText = FText::AsNumber(ItemAmount);
		ItemAmountTextBlock->SetText(ItemAmountText);
	}


}
