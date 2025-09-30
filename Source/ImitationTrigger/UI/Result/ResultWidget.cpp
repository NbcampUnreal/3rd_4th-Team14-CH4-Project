// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Result/ResultWidget.h"

#include "Components/Image.h"
#include "Components/TextBlock.h"

void UResultWidget::SetResult(int32 TotalPlayers, int32 PlayerRank, UTexture2D* CharacterImage, FText PlayerName,
                              int32 SurvivalTime, int32 KillCount, float Damage)
{

	if (PlayerRankBlock)
	{
		FString RankText = FString::Printf(TEXT("%d / %d"), PlayerRank, TotalPlayers );
		PlayerRankBlock->SetText(FText::FromString(RankText));
	}

	
	if (PlayerImage)
	{
		FSlateBrush Brush = PlayerImage->GetBrush();
		Brush.SetResourceObject(CharacterImage);
		PlayerImage->SetBrush(Brush);
	}

	if (PlayerNameBlock)
	{
		PlayerNameBlock->SetText(PlayerName);
	}

	if (SurvivalTimeBlock)
	{
		int32 Minutes = SurvivalTime / 60;
		int32 Seconds = SurvivalTime % 60;

		FString Time = FString::Printf(TEXT("%d:%02d"), Minutes, Seconds);
	}

	if (KillCountBlock)
	{
		FText KillNumber = FText::AsNumber(KillCount);
		KillCountBlock->SetText(KillNumber);
	}

	if (DamageBlock)
	{
		FText DamageNumber = FText::AsNumber(Damage);
		DamageBlock->SetText(DamageNumber);
	}
	
}
