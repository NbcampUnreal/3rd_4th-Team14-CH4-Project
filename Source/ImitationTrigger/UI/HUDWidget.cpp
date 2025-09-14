// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HUDWidget.h"

#include "PlayerBar/PlayerBarWidget.h"
#include "UltimateGauge/UltimateGaugeWidget.h"
#include "WeaponSlot/WeaponSlotNumberWidget.h"
#include "WeaponSlot/WeaponSlotWidget.h"


void UHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();

	UpdateUltimateGauge(0);
}

void UHUDWidget::UpdateUltimateGauge(float UltimateGauge)
{
	if (UltimateGauge >= 100)
	{
		UltimateGaugeWidget->ChargedUltimateGauge();
	}
	else
	{
		UltimateGaugeWidget->UltimateGauge(UltimateGauge);
	}
}

void UHUDWidget::UseUltimateGauge()
{
	UltimateGaugeWidget->UseUltimate();
}

void UHUDWidget::UpdateHealth(float CurrentHealth, float MaxHealth)
{
	LocalPlayerBar->UpdateHealthBar(CurrentHealth,MaxHealth);
}

void UHUDWidget::UpdateShield(float CurrentShield, float MaxShield)
{
	LocalPlayerBar->UpdateShieldBar(CurrentShield,MaxShield);
}

void UHUDWidget::WeaponSlotOne(UTexture2D* WeaponImage)
{
	WeaponSlot->SetWeaponImage(WeaponImage);
	WeaponSlotNumber2->SetRenderOpacity(0.6f);
	WeaponSlotNumber1->SetRenderOpacity(1.0f);
	
}

void UHUDWidget::WeaponSlotTwo(UTexture2D* WeaponImage)
{
	WeaponSlot->SetWeaponImage(WeaponImage);
	WeaponSlotNumber1->SetRenderOpacity(0.6f);
	WeaponSlotNumber2->SetRenderOpacity(1.0f);
}

void UHUDWidget::SetWeaponOneInfo(FText WeaponName)
{
	WeaponSlotNumber1->SetWeaponText(WeaponName);
}

void UHUDWidget::SetWeaponTwoInfo(FText WeaponName)
{
	WeaponSlotNumber2->SetWeaponText(WeaponName);
}
