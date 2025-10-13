// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WeaponSlot/WeaponSlotWidget.h"
#include "Components/Image.h"
#include "Engine/Texture2D.h"
#include "Styling/SlateBrush.h"

void UWeaponSlotWidget::SetWeaponImage(UTexture2D* NewImage)
{
	if (WeaponImage)
	{
		FSlateBrush Brush = WeaponImage->GetBrush();
		Brush.SetResourceObject(NewImage);
		WeaponImage->SetBrush(Brush);
		HasWeapon(true);
	}
}

void UWeaponSlotWidget::HasWeapon(bool bHasWeapon)
{
	if (!bHasWeapon)
	{
		if (WeaponImage)
		{
			WeaponImage->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
	else
	{
		if (WeaponImage)
		{
			WeaponImage->SetVisibility(ESlateVisibility::Visible);
		}
	}
}
