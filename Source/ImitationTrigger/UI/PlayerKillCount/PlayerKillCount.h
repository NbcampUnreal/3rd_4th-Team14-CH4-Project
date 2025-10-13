// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerKillCount.generated.h"

class UTextBlock;
/**
 * 
 */
UCLASS()
class IMITATIONTRIGGER_API UPlayerKillCount : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "UI")
	void UpdateRemainingPlayer(int32 RemainingPlayerCount);

	UFUNCTION(BlueprintCallable, Category = "UI")
	void UpdatePlayerKillCount(int32 PlayerKillCount);

protected:

	UPROPERTY(meta = (BindWidget))
	UTextBlock* RemainingPlayerCountBlock;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* PlayerKillCountBlock;
	
};
