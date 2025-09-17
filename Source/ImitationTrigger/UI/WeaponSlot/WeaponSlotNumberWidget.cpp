// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WeaponSlot/WeaponSlotNumberWidget.h"

#include "Components/TextBlock.h"


void UWeaponSlotNumberWidget::SetWeaponText(FText WeaponText)
{
	if (WeaponName)
	{
		WeaponName->SetText(WeaponText);
	}
}
