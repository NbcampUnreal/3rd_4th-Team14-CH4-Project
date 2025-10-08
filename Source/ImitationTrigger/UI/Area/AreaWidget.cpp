// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Area/AreaWidget.h"

#include "Components/TextBlock.h"

void UAreaWidget::SetAreaText(int32 CurrentRoundNumber, int32 AreaTime, float Distance, bool bIsWait)
{
	if (RoundTextBlock)
	{
		if (bIsWait)
		{
			FString RoundText = FString::Printf(TEXT("라운드 %d 대기"), CurrentRoundNumber);
			RoundTextBlock->SetText(FText::FromString(RoundText));
		}
		else
		{
			FString RoundText = FString::Printf(TEXT("라운드 %d 구역 축소"), CurrentRoundNumber);
			RoundTextBlock->SetText(FText::FromString(RoundText));
		}
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
