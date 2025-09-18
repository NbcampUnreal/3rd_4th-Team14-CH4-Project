// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HUDWidget.h"

#include "Components/Image.h"
#include "Components/ScrollBox.h"
#include "Components/VerticalBox.h"
#include "KillLog/KillLogWidget.h"
#include "KillNotify/KillNotifyWidget.h"
#include "PlayerBar/PlayerBarWidget.h"
#include "UltimateGauge/UltimateGaugeWidget.h"
#include "WeaponSlot/WeaponSlotNumberWidget.h"
#include "WeaponSlot/WeaponSlotWidget.h"
#include "Engine/TextureRenderTarget2D.h"

void UHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();

	UpdateUltimateGauge(0);

}

void UHUDWidget::UpdateUltimateGauge(float UltimateGauge)
{
	if (UltimateGauge >= 100)
	{
		UltimateGaugeWidget->OnUltimateGaugeCharged();
	}
	else
	{
		UltimateGaugeWidget->UpdateUltimateGauge(UltimateGauge);
	}
}

void UHUDWidget::ResetUltimateGauge()
{
	UltimateGaugeWidget->ResetUltimate();
}

void UHUDWidget::SetLocalPlayerBar(FText PlayerName, UTexture2D* PlayerIcon)
{
	LocalPlayerBar->UpdateCharacterImage(PlayerIcon);
	LocalPlayerBar->UpdateNameText(PlayerName);
}

void UHUDWidget::UpdateHealth(float CurrentHealth, float MaxHealth)
{
	LocalPlayerBar->UpdateHealthBar(CurrentHealth,MaxHealth);
}

void UHUDWidget::UpdateShield(float CurrentShield, float MaxShield)
{
	LocalPlayerBar->UpdateShieldBar(CurrentShield,MaxShield);
}

void UHUDWidget::UpdateWeaponSlotOne(UTexture2D* WeaponImage)
{
	WeaponSlot->SetWeaponImage(WeaponImage);
	WeaponSlotNumber2->SetRenderOpacity(0.6f);
	WeaponSlotNumber1->SetRenderOpacity(1.0f);
	
}

void UHUDWidget::UpdateWeaponSlotTwo(UTexture2D* WeaponImage)
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

void UHUDWidget::AddKillLog(UTexture2D* KillCharacter, FText KillName, UTexture2D* DieCharacter, FText DieName, UTexture2D* KillWeapon)
{

	if (!KillLogWidgetClass)
	{
		return;
	}

	UKillLogWidget* NewKillLog = CreateWidget<UKillLogWidget>(this,KillLogWidgetClass);

	if (NewKillLog && KillLogBox)
	{
		NewKillLog->SetKillLog(KillCharacter,KillName, DieCharacter, DieName, KillWeapon);
		KillLogBox->AddChild(NewKillLog);
		KillLogBox->ScrollToEnd();
		
	}
	
}

void UHUDWidget::AddNotifyText(FText KillPlayer, FText DiePlayer)
{
	if (!KillNotifyTextWidgetClass)
	{
		return;
	}

	UKillNotifyWidget* NewKillNotify = CreateWidget<UKillNotifyWidget>(this,KillNotifyTextWidgetClass);

	if (NewKillNotify && KillNotifyBox)
	{
		NewKillNotify->SetNotify(KillPlayer,DiePlayer);
		KillNotifyBox->AddChild(NewKillNotify);
		KillNotifyBox->ScrollToEnd();
	}
	
}

