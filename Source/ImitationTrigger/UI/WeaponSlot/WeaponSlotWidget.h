// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WeaponSlotWidget.generated.h"

class UImage;
/**
 * 
 */
UCLASS()
class IMITATIONTRIGGER_API UWeaponSlotWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable,Category = "UI")
	void SetWeaponImage(UTexture2D* NewImage);

	UFUNCTION(BlueprintCallable,Category = "UI")
	void HasWeapon(bool bHasWeapon);

protected:
	UPROPERTY(meta = (BindWidget))
	UImage* WeaponImage;
	
};
