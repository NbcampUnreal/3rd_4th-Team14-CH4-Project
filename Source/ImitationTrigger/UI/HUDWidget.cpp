// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HUDWidget.h"

#include "Components/Border.h"
#include "Components/Image.h"
#include "Components/ScrollBox.h"
#include "Components/VerticalBox.h"
#include "EquipmentIcon/EquipmentIconWidget.h"
#include "GameFramework/Character.h"
#include "KillLog/KillLogWidget.h"
#include "KillNotify/KillNotifyWidget.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerBar/PlayerBarWidget.h"
#include "UltimateGauge/UltimateGaugeWidget.h"
#include "WeaponSlot/WeaponSlotNumberWidget.h"
#include "WeaponSlot/WeaponSlotWidget.h"

void UHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();

	UpdateUltimateGauge(0);
	PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(),0);
	
}

void UHUDWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (!PlayerCharacter)
	{
		return;
	}

	if (FireCount > 0 && GetWorld()->GetTimeSeconds() - LastFireTime >= 1.5f)
	{
		bIsFire = false;
		FireCount = 0;
	}

	if (bIsFire)
	{
		if (FireSpread < MaxFireSpread)
		{
			FireSpread += 200 * InDeltaTime;
		}
	}
	else
	{
		if (FireSpread > 0)
		{
			FireSpread -= 100 * InDeltaTime;
		}
	}


	
	if (PlayerCharacter->GetVelocity().Size() > 300)
	{
		if (Spread < MaxWalkSpread)
		{
			Spread += 100 * InDeltaTime;
		}
	}
	else
	{
		Spread -= 100 * InDeltaTime;
	}

	FireSpread = FMath::Clamp(FireSpread,0.0f, MaxFireSpread);
	Spread = FMath::Clamp(Spread, 0.0f, MaxSpread);

	SetAimMaker(Spread);
	
}


void UHUDWidget::SetAimMaker(float Value)
{
	AimMakerUp->SetRenderTranslation(FVector2d(0,-(Value + FireSpread)));
	AimMakerDown->SetRenderTranslation(FVector2d(0,(Value + FireSpread)));
	AimMakerLeft->SetRenderTranslation(FVector2d(-(Value + FireSpread),0));
	AimMakerRight->SetRenderTranslation(FVector2d((Value + FireSpread),0));
}

void UHUDWidget::OnFire()
{
	if (FireCount < MaxFireCount)
	{
		FireCount++;
	}
	LastFireTime = GetWorld()->GetTimeSeconds();
	MaxFireSpread = 10 * FireCount;
	bIsFire = true;
}

void UHUDWidget::PlayHitMarkerAnimation()
{
	if (HitMarkerAnimation)
	{
		PlayAnimation(HitMarkerAnimation);
	}
}

void UHUDWidget::PlayKillMarkerAnimation()
{
	if (KillMarkerAnimation)
	{
		PlayAnimation(KillMarkerAnimation);
	}
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

void UHUDWidget::SetEquipmentIconBag()
{
	EquipmentIcon_Bag->SetCommonItem();
}

void UHUDWidget::SetEquipmentIconArmor()
{
	EquipmentIcon_Armor->SetEpicItem();
}

void UHUDWidget::SetEquipmentIconHelmet()
{
	EquipmentIcon_Helmet->SetRareItem();
}


