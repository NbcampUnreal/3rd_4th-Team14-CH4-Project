// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EquipmentIconWidget.generated.h"

class UImage;
/**
 * 
 */
UCLASS()
class IMITATIONTRIGGER_API UEquipmentIconWidget : public UUserWidget
{
	GENERATED_BODY()


public:
	
	UEquipmentIconWidget(const FObjectInitializer& ObjectInitializer);

	void SetItemRarity(int32 Rarity);


protected:

	UPROPERTY(meta = (BindWidget))
	UImage* EquipmentIconBackGround;

	UPROPERTY(editanywhere, BlueprintReadOnly)
	FLinearColor CommonColor;
	
	UPROPERTY(editanywhere, BlueprintReadOnly)
	FLinearColor RareColor;
	
	UPROPERTY(editanywhere, BlueprintReadOnly)
	FLinearColor EpicColor;

	
	
};
