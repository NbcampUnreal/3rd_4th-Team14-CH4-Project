// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/KillLog/KillLogWidget.h"

#include "Components/Image.h"
#include "Components/TextBlock.h"

void UKillLogWidget::SetKillLog(UTexture2D* KillCharacter, FText KillName, UTexture2D* KillWeapon, FText DieName, UTexture2D* DieCharacter)
{

	if (KillPlayerImage)
	{
		FSlateBrush Brush = KillPlayerImage->GetBrush();
		Brush.SetResourceObject(KillCharacter);
		KillPlayerImage->SetBrush(Brush);
	}
	
	if (KillPlayerName)
	{
		KillPlayerName->SetText(KillName);
	}

	if (KillWeaponImage)
	{
		FSlateBrush Brush = KillWeaponImage->GetBrush();
		Brush.SetResourceObject(KillWeapon);
		KillWeaponImage->SetBrush(Brush);
	}

	if (DiePlayerName)
	{
		DiePlayerName->SetText(DieName);
	}

	if (DiePlayerImage)
	{
		FSlateBrush Brush = DiePlayerImage->GetBrush();
		Brush.SetResourceObject(DieCharacter);
		DiePlayerImage->SetBrush(Brush);
	}
}
