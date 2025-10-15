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
	void UpdateUltimateGauge(float UltimateGauge);

	UFUNCTION(BlueprintCallable, Category = "UI")
	void OnUltimateGaugeCharged();

	UFUNCTION(BlueprintCallable, Category = "UI")
	void ResetUltimate();

	UFUNCTION(BlueprintCallable, Category = "UI")
	void SetIconImage(UTexture2D* Icon);
	
protected:
	virtual void NativeConstruct() override;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* UltimateText;

	UPROPERTY(meta = (BindWidget))
	UImage* IconImage;

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
