// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "KillLogWidget.generated.h"

class UTextBlock;
class UImage;
/**
 * 
 */
UCLASS()
class IMITATIONTRIGGER_API UKillLogWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	UFUNCTION(BlueprintCallable, Category = "KillLogWidget")
	void SetKillLog(UTexture2D* KillCharacter, FText KillName, FText DieName, UTexture2D* DieCharacter, UTexture2D* KillWeapon);
	


protected:

	UPROPERTY(meta = (BindWidget))
	UImage* KillPlayerImage;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* KillPlayerName;

	UPROPERTY(meta = (BindWidget))
	UImage* KillWeaponImage;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* DiePlayerName;

	UPROPERTY(meta = (BindWidget))
	UImage* DiePlayerImage;
};
