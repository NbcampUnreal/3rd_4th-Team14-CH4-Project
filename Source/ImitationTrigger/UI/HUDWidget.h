// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HUDWidget.generated.h"

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
	void UseUltimateGauge();

	UFUNCTION(BlueprintCallable, Category = "UI")
	void UpdateHealth(float CurrentHealth, float MaxHealth);

	UFUNCTION(BlueprintCallable, Category = "UI")
	void UpdateShield(float CurrentShield, float MaxShield);
	
protected:
	
	virtual void NativeConstruct() override;

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
	// UPROPERTY(meta = (BindWidget))
	// UWeaponSlotWidget* WeaponSlot;
	//
	// UPROPERTY(meta = (BindWidget))
	// UWeaponSlotNumberWidget* WeaponSlotNumber1;
	//
	// UPROPERTY(meta = (BindWidget))
	// UWeaponSlotNumberWidget* WeaponSlotNumber2;
	
};
