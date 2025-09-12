// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UltimateGauge/UltimateGaugeWidget.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"


void UUltimateGaugeWidget::NativeConstruct()
{
	Super::NativeConstruct();
	UpdateUltimateGauge(0);
	if (UltimateImage)
	{
		UltimateDynamicMaterial = UltimateImage->GetDynamicMaterial();
	}
}

void UUltimateGaugeWidget::UpdateUltimateGauge(float UltimateGauge)
{
	float UltimateValue = FMath::Clamp(UltimateGauge, 0.0f, 100.0f);

	if (UltimateText)
	{
		FText GaugeText = FText::Format(
			FText::FromString("{0}%"),
			FText::AsNumber(UltimateValue)
			);
		UltimateText->SetText(GaugeText);
	}

	if (UltimateDynamicMaterial)
	{
		float ParamValue = UltimateValue * 0.01f;
		UltimateDynamicMaterial->SetScalarParameterValue(FName(TEXT("Radial_Wipe")),ParamValue);
	}

	
	
}


