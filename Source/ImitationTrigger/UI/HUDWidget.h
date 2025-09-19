// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HUDWidget.generated.h"

class UItemSlotWidget;
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
	void SetItemBag();

	UFUNCTION(BlueprintCallable, Category = "UI")
	void SetItemArmor();
	
	UFUNCTION(BlueprintCallable, Category = "UI")
	void SetItemHelmet();

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
	UItemSlotWidget* ItemSlot_Bag;

	UPROPERTY(meta = (BindWidget))
	UItemSlotWidget* ItemSlot_Armor;

	UPROPERTY(meta = (BindWidget))
	UItemSlotWidget* ItemSlot_Helmet;



private:

	UPROPERTY(EditDefaultsOnly, Category = "KillLog")
	TSubclassOf<UKillLogWidget> KillLogWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "KillNotify")
	TSubclassOf<UKillNotifyWidget> KillNotifyTextWidgetClass;
	
};
