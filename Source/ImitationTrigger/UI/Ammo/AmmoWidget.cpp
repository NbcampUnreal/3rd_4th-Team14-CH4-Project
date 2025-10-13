// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Ammo/AmmoWidget.h"

#include "Components/Image.h"
#include "Components/RichTextBlock.h"


void UAmmoWidget::SetRifleAmmo(int32 CurrentAmmo, int32 MaxAmmo)
{
	if (AmmoRichTextBlock)
	{
		FString AmmoText = FString::Printf(TEXT("%d / <RifleColor>%03d</>"), CurrentAmmo, MaxAmmo );
		AmmoRichTextBlock->SetText(FText::FromString(AmmoText));
	}
	if (AmmoImage)
	{
		AmmoImage->SetBrushFromTexture(RifleAmmoImage);
	}
	HasWeapon(true);
}

void UAmmoWidget::SetShotgunAmmo(int32 CurrentAmmo, int32 MaxAmmo)
{
	if (AmmoRichTextBlock)
	{
		FString AmmoText = FString::Printf(TEXT("%d / <ShotgunColor>%03d</>"), CurrentAmmo, MaxAmmo );
		AmmoRichTextBlock->SetText(FText::FromString(AmmoText));
	}
	if (AmmoImage)
	{
		AmmoImage->SetBrushFromTexture(ShotgunAmmoImage);
	}
	HasWeapon(true);
}

void UAmmoWidget::SetSniperAmmo(int32 CurrentAmmo, int32 MaxAmmo)
{
	if (AmmoRichTextBlock)
	{
		FString AmmoText = FString::Printf(TEXT("%d / <SniperColor>%03d</>"), CurrentAmmo, MaxAmmo );
		AmmoRichTextBlock->SetText(FText::FromString(AmmoText));
	}
	if (AmmoImage)
	{
		AmmoImage->SetBrushFromTexture(SniperAmmoImage);
	}
	HasWeapon(true);
}

void UAmmoWidget::HasWeapon(bool bHasWeapon)
{
	if (!bHasWeapon)
	{
		if (AmmoRichTextBlock)
		{
			AmmoRichTextBlock->SetRenderOpacity(0.5f);
			FString AmmoText = FString::Printf(TEXT("00 / 000"));
			AmmoRichTextBlock->SetText(FText::FromString(AmmoText));
			
		}
		if (AmmoImage)
		{
			AmmoImage->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
	else
	{
		if (AmmoRichTextBlock)
		{
			AmmoRichTextBlock->SetRenderOpacity(1.0f);
		}
		if (AmmoImage)
		{
			AmmoImage->SetVisibility(ESlateVisibility::Visible);
		}
	}
}
