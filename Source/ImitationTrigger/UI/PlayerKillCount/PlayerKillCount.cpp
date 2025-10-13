// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PlayerKillCount/PlayerKillCount.h"

#include "Components/TextBlock.h"

void UPlayerKillCount::UpdateRemainingPlayer(int32 RemainingPlayerCount)
{
	if (RemainingPlayerCountBlock)
	{
		FText PlayerCount = FText::AsNumber(RemainingPlayerCount);
		RemainingPlayerCountBlock->SetText(PlayerCount);
	}
}

void UPlayerKillCount::UpdatePlayerKillCount(int32 PlayerKillCount)
{
	if (PlayerKillCountBlock)
	{
		FText KillCount = FText::AsNumber(PlayerKillCount);
		PlayerKillCountBlock->SetText(KillCount);
	}
}
