// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HUDWidget.h"

#include "PlayerBar/PlayerBarWidget.h"
#include "UltimateGauge/UltimateGaugeWidget.h"

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
