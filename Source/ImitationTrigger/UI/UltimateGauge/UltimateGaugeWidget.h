// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UltimateGaugeWidget.generated.h"

class UImage;
class UTextBlock;
/**
 * 
 */
UCLASS()
class IMITATIONTRIGGER_API UUltimateGaugeWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void UpdateUltimateGauge(float UltimateGauge);

protected:
	virtual void NativeConstruct() override;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* UltimateText;

	UPROPERTY(meta = (BindWidget))
	UImage* UltimateImage;

private:
	UPROPERTY()
	UMaterialInstanceDynamic* UltimateDynamicMaterial;

	
};
