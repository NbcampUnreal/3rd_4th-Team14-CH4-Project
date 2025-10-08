// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HUDWidget.generated.h"

class USkillWidget;
class UAmmoWidget;
class UBorder;
class UEquipmentIconWidget;
class UVerticalBox;
class UKillNotifyWidget;
class UKillLogWidget;
class UScrollBox;
class UWeaponSlotNumberWidget;
class UWeaponSlotWidget;
class UPlayerBarWidget;
class UUltimateGaugeWidget;
class UTextBlock;
class UImage;
class UMaterialInstanceDynamic;


UCLASS()
class IMITATIONTRIGGER_API UHUDWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category = "UI")
	void UpdateUltimateGauge(float UltimateGauge);

	UFUNCTION(BlueprintCallable, Category = "UI")
	void ResetUltimateGauge();
	
	UFUNCTION(BlueprintCallable, Category = "UI")
	void SetLocalPlayerBar(FText PlayerName, UTexture2D* PlayerIcon);

	UFUNCTION(BlueprintCallable, Category = "UI")
	void UpdateHealth(float CurrentHealth, float MaxHealth);

	UFUNCTION(BlueprintCallable, Category = "UI")
	void UpdateShield(float CurrentShield, float MaxShield);

	UFUNCTION(BlueprintCallable, Category = "UI")
	void UpdateWeaponSlotOne(UTexture2D* WeaponImage);

	UFUNCTION(BlueprintCallable, Category = "UI")
	void UpdateWeaponSlotTwo(UTexture2D* WeaponImage);

	UFUNCTION(BlueprintCallable, Category = "UI")
	void SetWeaponOneInfo(FText WeaponName);

	UFUNCTION(BlueprintCallable, Category = "UI")
	void SetWeaponTwoInfo(FText WeaponName);

	UFUNCTION(BlueprintCallable, Category = "UI")
	void AddKillLog(UTexture2D* KillCharacter, FText KillName, UTexture2D* DieCharacter, FText DieName, UTexture2D* KillWeapon);

	UFUNCTION(BlueprintCallable, Category = "UI")
	void AddNotifyText(FText KillPlayer, FText DiePlayer);

	UFUNCTION(BlueprintCallable, Category = "UI")
	void SetEquipmentIconBag();

	UFUNCTION(BlueprintCallable, Category = "UI")
	void SetEquipmentIconArmor();
	
	UFUNCTION(BlueprintCallable, Category = "UI")
	void SetEquipmentIconHelmet();

	UFUNCTION(BlueprintCallable, Category = "UI")
	void SetAimMaker(float Value);

	UFUNCTION(BlueprintCallable, Category = "UI")
	void OnFire();

	UFUNCTION(BlueprintCallable, Category = "UI")
	void PlayHitMarkerAnimation();
	
	UFUNCTION(BlueprintCallable, Category = "UI")
	void PlayKillMarkerAnimation();

	UFUNCTION(BlueprintCallable, Category = "UI")
	void UpdateRifleAmmo(int32 CurrentAmmo, int32 MaxAmmo);
	
	UFUNCTION(BlueprintCallable, Category = "UI")
	void UpdateShotgunAmmo(int32 CurrentAmmo, int32 MaxAmmo);
	
	UFUNCTION(BlueprintCallable, Category = "UI")
	void UpdateSniperAmmo(int32 CurrentAmmo, int32 MaxAmmo);
	
	UFUNCTION(BlueprintCallable, Category = "UI")
	void HasWeapon(bool bHasWeapon);

	UFUNCTION(BlueprintCallable, Category = "UI")
	void OnSkill(float CoolDown);
	
protected:
	
	virtual void NativeConstruct() override;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	UPROPERTY(meta = (BindWidget))
	UUltimateGaugeWidget* UltimateGaugeWidget;

	UPROPERTY(meta = (BindWidget))
	UPlayerBarWidget* LocalPlayerBar;
	
	// UPROPERTY(meta = (BindWidget))
	// UPlayerBarWidget* TeamPlayer1Bar;
	//
	// UPROPERTY(meta = (BindWidget))
	// UPlayerBarWidget* TeamPlayer2Bar;
	//
	// UPROPERTY(meta = (BindWidget))
	// UPlayerBarWidget* TeamPlayer3Bar;
	//
	UPROPERTY(meta = (BindWidget))
	UWeaponSlotWidget* WeaponSlot;
	
	UPROPERTY(meta = (BindWidget))
	UWeaponSlotNumberWidget* WeaponSlotNumber1;
	
	UPROPERTY(meta = (BindWidget))
	UWeaponSlotNumberWidget* WeaponSlotNumber2;

	UPROPERTY(meta = (BindWidget))
	UScrollBox* KillLogBox;

	UPROPERTY(meta = (BindWidget))
	UScrollBox* KillNotifyBox;

	UPROPERTY(meta = (BindWidget))
	UEquipmentIconWidget* EquipmentIcon_Bag;

	UPROPERTY(meta = (BindWidget))
	UEquipmentIconWidget* EquipmentIcon_Armor;

	UPROPERTY(meta = (BindWidget))
	UEquipmentIconWidget* EquipmentIcon_Helmet;

	UPROPERTY(meta = (BindWidget))
	UBorder* AimMakerUp;

	UPROPERTY(meta = (BindWidget))
	UBorder* AimMakerDown;

	UPROPERTY(meta = (BindWidget))
	UBorder* AimMakerLeft;

	UPROPERTY(meta = (BindWidget))
	UBorder* AimMakerRight;

	UPROPERTY()
	ACharacter* PlayerCharacter;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* HitMarkerAnimation;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* KillMarkerAnimation;

	UPROPERTY(meta = (BindWidget))
	UAmmoWidget* AmmoWidget;

	UPROPERTY(meta = (BindWidget))
	USkillWidget* SkillSlotWidget;


private:

	UPROPERTY(EditDefaultsOnly, Category = "KillLog")
	TSubclassOf<UKillLogWidget> KillLogWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "KillNotify")
	TSubclassOf<UKillNotifyWidget> KillNotifyTextWidgetClass;

	UPROPERTY()
	float WalkSpread = 0.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Spread")
	float MaxWalkSpread = 25.0f;

	UPROPERTY()
	int32 FireSpreadCount = 0;

	UPROPERTY(EditDefaultsOnly, Category = "Spread")
	float FireSpreadAmount = 5;

	UPROPERTY(EditDefaultsOnly, Category = "Spread")
	int32 MaxFireSpreadCount = 5;

	UPROPERTY()
	float FireSpread = 0.0f;

	UPROPERTY()
	float MaxFireSpread = 0.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Spread")
	float IncreaseFireSpreadSpeed = 100.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Spread")
	float DecreaseFireSpreadSpeed = 200.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Spread")
	float IncreaseSpreadSpeed = 100.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Spread")
	float DecreaseSpreadSpeed = 100.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Spread")
	float SpreadRecoveryTime = 1.5f;

	UPROPERTY(EditDefaultsOnly, Category = "Spread")
	float MaxSpread = 50.0f;

	UPROPERTY()
	float LastFireTime = 0.0f;

	UPROPERTY()
	bool bIsFire = false;
	
};
