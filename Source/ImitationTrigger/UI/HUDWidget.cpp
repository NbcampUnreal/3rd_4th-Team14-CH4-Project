// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HUDWidget.h"

#include "Ammo/AmmoWidget.h"
#include "Components/Border.h"
#include "Components/Image.h"
#include "Components/ScrollBox.h"
#include "EquipmentIcon/EquipmentIconWidget.h"
#include "GameFramework/Character.h"
#include "KillLog/KillLogWidget.h"
#include "KillNotify/KillNotifyWidget.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerBar/PlayerBarWidget.h"
#include "SkillSlot/SkillWidget.h"
#include "UltimateGauge/UltimateGaugeWidget.h"
#include "WeaponSlot/WeaponSlotNumberWidget.h"
#include "WeaponSlot/WeaponSlotWidget.h"
#include "Area/AreaWidget.h"
#include "ItemSlot/ItemSlotWidget.h"
#include "PlayerKillCount/PlayerKillCount.h"

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

	if (FireSpreadCount > 0 && GetWorld()->GetTimeSeconds() - LastFireTime >= SpreadRecoveryTime)
	{
		bIsFire = false;
		FireSpreadCount = 0;
	}

	if (bIsFire)
	{
		if (FireSpread < MaxFireSpread)
		{
			FireSpread += IncreaseFireSpreadSpeed * InDeltaTime;
		}
	}
	else
	{
		if (FireSpread > 0)
		{
			FireSpread -= DecreaseFireSpreadSpeed * InDeltaTime;
		}
	}


	
	if (PlayerCharacter->GetVelocity().Size() > 300)
	{
		if (WalkSpread < MaxWalkSpread)
		{
			WalkSpread += IncreaseSpreadSpeed * InDeltaTime;
		}
	}
	else
	{
		WalkSpread -= DecreaseSpreadSpeed * InDeltaTime;
	}

	FireSpread = FMath::Clamp(FireSpread, 0.0f, MaxFireSpread);
	WalkSpread = FMath::Clamp(WalkSpread, 0.0f, MaxWalkSpread);

	float TotalSpread = WalkSpread + FireSpread;
	TotalSpread = FMath::Clamp(TotalSpread, 0.0f, MaxSpread);
	
	SetAimMaker(TotalSpread);
	
}


void UHUDWidget::SetAimMaker(float Value)
{
	AimMakerUp->SetRenderTranslation(FVector2d(0,-Value));
	AimMakerDown->SetRenderTranslation(FVector2d(0,Value));
	AimMakerLeft->SetRenderTranslation(FVector2d(-Value,0));
	AimMakerRight->SetRenderTranslation(FVector2d(Value,0));
}

void UHUDWidget::OnFire()
{
	if (FireSpreadCount < MaxFireSpreadCount)
	{
		FireSpreadCount++;
	}
	LastFireTime = GetWorld()->GetTimeSeconds();
	MaxFireSpread = FireSpreadAmount * FireSpreadCount;
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

void UHUDWidget::UpdateRifleAmmo(int32 CurrentAmmo, int32 MaxAmmo)
{
	if (AmmoWidget)
	{
		AmmoWidget->SetRifleAmmo(CurrentAmmo, MaxAmmo);
	}
}

void UHUDWidget::UpdateShotgunAmmo(int32 CurrentAmmo, int32 MaxAmmo)
{
	if (AmmoWidget)
	{
		AmmoWidget->SetShotgunAmmo(CurrentAmmo, MaxAmmo);
	}
}

void UHUDWidget::UpdateSniperAmmo(int32 CurrentAmmo, int32 MaxAmmo)
{
	if (AmmoWidget)
	{
		AmmoWidget->SetSniperAmmo(CurrentAmmo, MaxAmmo);
	}
}

void UHUDWidget::HasWeapon(bool bHasWeapon)
{
	if (AmmoWidget)
	{
		AmmoWidget->HasWeapon(bHasWeapon);
	}
}

void UHUDWidget::OnSkill(float CoolDown)
{
	if (SkillSlotWidget)
	{
		SkillSlotWidget->OnSkill(CoolDown);
	}
}

void UHUDWidget::OnUpdateAreaInfo(int32 CurrentRoundNumber, int32 AreaTime, float Distance, bool bIsWait)
{
	if (AreaWidget)
	{
		AreaWidget->SetAreaText(CurrentRoundNumber, AreaTime, Distance, bIsWait);
	}
}

void UHUDWidget::UpdateRemainingPlayer(int32 RemainingPlayerCount)
{
	if (PlayerKillCountWidget)
	{
		PlayerKillCountWidget->UpdateRemainingPlayer(RemainingPlayerCount);
	}
}

void UHUDWidget::UpdatePlayerKillCount(int32 PlayerKillCount)
{
	if (PlayerKillCountWidget)
	{
		PlayerKillCountWidget->UpdatePlayerKillCount(PlayerKillCount);
	}
}

void UHUDWidget::UpdateItemAmount(int32 ItemAmount)
{
	if (ItemSlotWidget)
	{
		ItemSlotWidget->UpdateItemAmount(ItemAmount);
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


