// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "KillNotifyWidget.generated.h"

class UTextBlock;
/**
 * 
 */
UCLASS()
class IMITATIONTRIGGER_API UKillNotifyWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	UFUNCTION(BlueprintCallable, Category = "KillNotify")
	void SetNotify(FText KillPlayer, FText DiePlayer);

protected:

	UPROPERTY(meta = (BindWidget))
	UTextBlock* KillNotifyText;

	
};
