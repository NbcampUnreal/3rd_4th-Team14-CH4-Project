// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ResultWidget.generated.h"

class UImage;
class UTextBlock;
/**
 * 
 */
UCLASS()
class IMITATIONTRIGGER_API UResultWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category = "UI")
	void SetResult(int32 TotalPlayers, int32 MyRank, UTexture2D* CharacterImage, FText PlayerName, int32 SurvivalTime, int32 KillCount, float Damage);


protected:

	UPROPERTY(meta = (BindWidget))
	UTextBlock* PlayerRankBlock;

	UPROPERTY(meta = (BindWidget))
	UImage* PlayerImage;
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock* PlayerNameBlock;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* SurvivalTimeBlock;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* KillCountBlock;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* DamageBlock;

	
};
