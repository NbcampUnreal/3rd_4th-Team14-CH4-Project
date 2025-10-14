// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UltimateGauge/UltimateGaugeWidget.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"



void UUltimateGaugeWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	UltimateDynamicMaterial = GetImageDynamicMaterial(UltimateImage);
	UltimateEffectDynamicMaterial = GetImageDynamicMaterial(UltimateEffect);
}

UMaterialInstanceDynamic* UUltimateGaugeWidget::GetImageDynamicMaterial(UImage* Image)
{
	if (Image)
	{
		return Image->GetDynamicMaterial();
	}
	return nullptr;
}

void UUltimateGaugeWidget::UpdateUltimateGauge(float UltimateGauge)
{
	float UltimateValue = FMath::Clamp(UltimateGauge, 0.0f, 100.0f);
	int32 IntValue = (int32)UltimateValue;

	if (UltimateText)
	{
		FText GaugeText = FText::Format(
			FText::FromString("{0}%"),
			FText::AsNumber(IntValue)
			);
		UltimateText->SetText(GaugeText);
	}

	if (UltimateDynamicMaterial)
	{
		float ParamValue = UltimateValue * 0.01f;
		UltimateDynamicMaterial->SetScalarParameterValue(FName(TEXT("Radial_Wipe")),ParamValue);
	}
}

void UUltimateGaugeWidget::OnUltimateGaugeCharged()
{
	if (UltimateText)
	{
		UltimateText->SetVisibility(ESlateVisibility::Collapsed);
	}

	if (UltimateImage)
	{
		if (UltimateDynamicMaterial)
		{
			UltimateDynamicMaterial->SetScalarParameterValue(FName(TEXT("Block_Ring")),1);
		}
	}

	if (UltimateEffect)
	{
		UltimateEffect->SetVisibility(ESlateVisibility::Visible);
	}
}

void UUltimateGaugeWidget::ResetUltimate()
{
	if (UltimateText)
	{
		UltimateText->SetVisibility(ESlateVisibility::Visible);
	}

	if (UltimateImage)
	{
		if (UltimateDynamicMaterial)
		{
			UltimateDynamicMaterial->SetScalarParameterValue(FName(TEXT("Block_Ring")),0.85);
		}
	}

	if (UltimateEffect)
	{
		UltimateEffect->SetVisibility(ESlateVisibility::Collapsed);
	}
}


