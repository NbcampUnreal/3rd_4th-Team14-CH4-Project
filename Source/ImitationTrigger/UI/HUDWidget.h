// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HUDWidget.generated.h"

class UTextBlock;
class UImage;
class UMaterialInstanceDynamic;

UCLASS()
class IMITATIONTRIGGER_API UHUDWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category = "UI")
	void UpdateUltimateGauge(float UltimateGauge);


	
protected:
	
	virtual void NativeConstruct() override;

private:

	UPROPERTY()
	UMaterialInstanceDynamic* DynamicMaterial;

	

	
};
