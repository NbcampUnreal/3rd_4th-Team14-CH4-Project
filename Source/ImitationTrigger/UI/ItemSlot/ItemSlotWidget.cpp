// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ItemSlot/ItemSlotWidget.h"

#include "Components/Image.h"

void UItemSlotWidget::SetCommonItem()
{
	FLinearColor NewColor = FLinearColor::White;

	if (ItemSlot_0)
	{
		ItemSlot_0->SetBrushTintColor(FSlateColor(NewColor));
	}
}

void UItemSlotWidget::SetRareItem()
{
	FLinearColor NewColor = FLinearColor::Green;

	if (ItemSlot_0)
	{
		ItemSlot_0->SetBrushTintColor(FSlateColor(NewColor));
	}
}

void UItemSlotWidget::SetEpicItem()
{
	FLinearColor NewColor = FLinearColor::Red + FLinearColor::Blue;

	if (ItemSlot_0)
	{
		ItemSlot_0->SetBrushTintColor(FSlateColor(NewColor));
	}
}
