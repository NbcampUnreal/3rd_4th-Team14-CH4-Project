// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WeaponSlotNumberWidget.generated.h"

class UTextBlock;
/**
 * 
 */
UCLASS()
class IMITATIONTRIGGER_API UWeaponSlotNumberWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "UI")
	void SetWeaponText(FText& WeaponText);


protected:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* WeaponName;

	
};
