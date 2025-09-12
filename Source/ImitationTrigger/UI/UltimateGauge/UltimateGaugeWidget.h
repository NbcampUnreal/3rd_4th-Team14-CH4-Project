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

	UFUNCTION(BlueprintCallable, Category = "UI")
	void UltimateGauge(float UltimateGauge);

	UFUNCTION(BlueprintCallable, Category = "UI")
	void ChargedUltimateGauge();

	UFUNCTION(BlueprintCallable, Category = "UI")
	void UseUltimate();
	
protected:
	virtual void NativeConstruct() override;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* UltimateText;

	UPROPERTY(meta = (BindWidget))
	UImage* UltimateImage;

	UPROPERTY(meta = (BindWidget))
	UImage* UltimateEffect;

	UMaterialInstanceDynamic* GetImageDynamicMaterial(UImage* Image);

private:
	UPROPERTY()
	UMaterialInstanceDynamic* UltimateDynamicMaterial;
	UPROPERTY()
	UMaterialInstanceDynamic* UltimateEffectDynamicMaterial;

	
};
