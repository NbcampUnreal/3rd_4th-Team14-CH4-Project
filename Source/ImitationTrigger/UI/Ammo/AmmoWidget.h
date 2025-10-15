// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AmmoWidget.generated.h"


class URichTextBlock;
class UImage;
/**
 * 
 */
UCLASS()
class IMITATIONTRIGGER_API UAmmoWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable)
	void SetRifleAmmo(int32 CurrentAmmo, int32 MaxAmmo);
	UFUNCTION(BlueprintCallable)
	void SetShotgunAmmo(int32 CurrentAmmo, int32 MaxAmmo);
	UFUNCTION(BlueprintCallable)
	void SetSniperAmmo(int32 CurrentAmmo, int32 MaxAmmo);
	UFUNCTION(BlueprintCallable)
	void HasWeapon(bool bHasWeapon);


protected:

	UPROPERTY(meta = (BindWidget))
	UImage* AmmoImage;

	UPROPERTY(meta = (BindWidget))
	URichTextBlock* AmmoRichTextBlock;

private:

	UPROPERTY(EditDefaultsOnly)
	UTexture2D* RifleAmmoImage;

	UPROPERTY(EditDefaultsOnly)
	UTexture2D* ShotgunAmmoImage;

	UPROPERTY(EditDefaultsOnly)
	UTexture2D* SniperAmmoImage;

	
};
