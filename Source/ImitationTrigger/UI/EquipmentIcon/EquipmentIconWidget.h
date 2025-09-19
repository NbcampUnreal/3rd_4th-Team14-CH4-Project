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
	
	void SetCommonItem();

	void SetRareItem();

	void SetEpicItem();


protected:

	UPROPERTY(meta = (BindWidget))
	UImage* EquipmentIcon_0;

	UPROPERTY(meta = (BindWidget))
	UImage* EquipmentIcon_1;
};
