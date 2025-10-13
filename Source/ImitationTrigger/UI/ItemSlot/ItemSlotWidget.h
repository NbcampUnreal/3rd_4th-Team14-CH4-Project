// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ItemSlotWidget.generated.h"

class UTextBlock;
class UImage;
/**
 * 
 */
UCLASS()
class IMITATIONTRIGGER_API UItemSlotWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category = "UI")
	void UpdateItemAmount(int32 ItemAmount);


	
protected:

	UPROPERTY(meta = (BindWidget))
	UImage* ItemImage;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* ItemAmountTextBlock;

private:
	
};
