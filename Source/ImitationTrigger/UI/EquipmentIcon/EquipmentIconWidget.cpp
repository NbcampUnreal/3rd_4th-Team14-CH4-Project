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

void UEquipmentIconWidget::SetItemRarity(int32 Rarity)
{
	if (!EquipmentIconBackGround)
	{
		return;
	}

	FLinearColor TargetColor = FLinearColor::White;
	
	switch (Rarity)
	{

		case 0:
			TargetColor = CommonColor;
			break;

		case 1:
			TargetColor = RareColor;
			break;

		case 2:
			TargetColor = EpicColor;
			break;

		default:
			return;
	}

	EquipmentIconBackGround->SetBrushTintColor(FSlateColor(TargetColor));
}
