// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/EquipmentIcon/EquipmentIconWidget.h"

#include "Components/Image.h"

void UEquipmentIconWidget::SetCommonItem()
{
	FLinearColor NewColor = FLinearColor::White;

	if (EquipmentIcon_0)
	{
		EquipmentIcon_0->SetBrushTintColor(FSlateColor(NewColor));
	}
}

void UEquipmentIconWidget::SetRareItem()
{
	FLinearColor NewColor = FLinearColor::Green;

	if (EquipmentIcon_0)
	{
		EquipmentIcon_0->SetBrushTintColor(FSlateColor(NewColor));
	}
}

void UEquipmentIconWidget::SetEpicItem()
{
	FLinearColor NewColor = FLinearColor::Red + FLinearColor::Blue;

	if (EquipmentIcon_0)
	{
		EquipmentIcon_0->SetBrushTintColor(FSlateColor(NewColor));
	}
}
