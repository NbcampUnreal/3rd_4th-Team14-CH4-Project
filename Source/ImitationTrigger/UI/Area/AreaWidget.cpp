// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Area/AreaWidget.h"

#include "Components/TextBlock.h"

void UAreaWidget::SetAreaText(int32 CurrentRoundNumber, int32 AreaTime, float Distance)
{
	if (RoundTextBlock)
	{
		FString RoundText = FString::Printf(TEXT("라운드 %d"), CurrentRoundNumber);
		RoundTextBlock->SetText(FText::FromString(RoundText));
	}

	if (TimeTextBlock)
	{
		int32 Minutes = AreaTime / 60;
		int32 Seconds = AreaTime % 60;
		
		FString TimeText = FString::Printf(TEXT("%02d:%02d"), Minutes, Seconds);
		TimeTextBlock->SetText(FText::FromString(TimeText));
	}

	if (DistanceTextBlock)
	{
		FString DistanceText = FString::Printf(TEXT("%.fM"), Distance);
		DistanceTextBlock->SetText(FText::FromString(DistanceText));
	}
	
}
