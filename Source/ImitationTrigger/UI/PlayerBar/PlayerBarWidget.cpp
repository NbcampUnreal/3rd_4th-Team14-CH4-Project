// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PlayerBar/PlayerBarWidget.h"

#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"


void UPlayerBarWidget::NativePreConstruct()
{
	Super::NativePreConstruct();

	if (PlayerNameText)
	{
		PlayerNameText->SetText(NameText);
	}

	
}

void UPlayerBarWidget::NativeConstruct()
{
	Super::NativeConstruct();
}


void UPlayerBarWidget::UpdateHealthBar(float CurrentHealth, float MaxHealth)
{
	if(MaxHealth > 0.0f)
	{
		HealthPercent = CurrentHealth / MaxHealth;
	}
	else
	{
		HealthPercent = 0.0f;
	}

	HealthPercent = FMath::Clamp(HealthPercent, 0.0f, 1.0f);

	if (HealthBar)
	{
		HealthBar->SetPercent(HealthPercent);
	}
	
}

void UPlayerBarWidget::UpdateShieldBar(float CurrentShield, float MaxShield)
{
	if(MaxShield > 0.0f)
	{
		ShieldPercent = CurrentShield / MaxShield;
	}
	else
	{
		ShieldPercent = 0.0f;
	}

	ShieldPercent = FMath::Clamp(ShieldPercent, 0.0f, 1.0f);

	if (ShieldBar)
	{
		ShieldBar->SetPercent(ShieldPercent);
	}
}


