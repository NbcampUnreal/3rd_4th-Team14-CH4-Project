// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/EquipmentIcon/EquipmentIconWidget.h"

#include "Components/Image.h"


UEquipmentIconWidget::UEquipmentIconWidget(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	CommonColor = FLinearColor::White;
	RareColor = FLinearColor::Green;
	EpicColor = FLinearColor::Red + FLinearColor::Blue;
}

void UEquipmentIconWidget::SetCommonItem()
{

	if (EquipmentIconBackGround)
	{
		EquipmentIconBackGround->SetBrushTintColor(FSlateColor(CommonColor));
	}
}

void UEquipmentIconWidget::SetRareItem()
{
	
	if (EquipmentIconBackGround)
	{
		EquipmentIconBackGround->SetBrushTintColor(FSlateColor(RareColor));
	}
}

void UEquipmentIconWidget::SetEpicItem()
{

	if (EquipmentIconBackGround)
	{
		EquipmentIconBackGround->SetBrushTintColor(FSlateColor(EpicColor));
	}
}
