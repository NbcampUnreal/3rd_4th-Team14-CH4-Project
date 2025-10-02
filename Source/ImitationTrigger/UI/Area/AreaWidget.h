// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AreaWidget.generated.h"

class USlider;
class UTextBlock;
/**
 * 
 */
UCLASS()
class IMITATIONTRIGGER_API UAreaWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable)
	void SetAreaText(int32 CurrentRoundNumber, int32 AreaTime, float Distance);

protected:
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock* RoundTextBlock;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* TimeTextBlock;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* DistanceTextBlock;

	
};
