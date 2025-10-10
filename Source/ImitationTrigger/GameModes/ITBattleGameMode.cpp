#include "GameModes/ITBattleGameMode.h"
#include "Engine/World.h"
#include "Engine/Engine.h"

AITBattleGameMode::AITBattleGameMode()
{
	bUseSeamlessTravel = false;
}

void AITBattleGameMode::BeginPlay()
{
	Super::BeginPlay();

	ExtractSessionInfoFromURL();

	UE_LOG(LogTemp, Log, TEXT("Match up. Session=%s Expect=%d"), *CurrentSessionID, ExpectedPlayerCount);
}

void AITBattleGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	MatchPlayers.AddUnique(NewPlayer);
	UE_LOG(LogTemp, Log, TEXT("Joined. %d/%d"), MatchPlayers.Num(), ExpectedPlayerCount);

	StartMatchWhenReady();
}

void AITBattleGameMode::RestartPlayer(AController* NewPlayer)
{
	Super::RestartPlayer(NewPlayer);
}


void AITBattleGameMode::ExtractSessionInfoFromURL()
{
	const FString URL = GetWorld() ? GetWorld()->URL.ToString() : FString();

	// SessionID
	{
		const FString Key = TEXT("SessionID=");
		int32 Index = URL.Find(Key, ESearchCase::IgnoreCase, ESearchDir::FromStart);
		if (Index != INDEX_NONE)
		{
			const int32 Start = Index + Key.Len();
			int32 End = URL.Find(TEXT("&"), ESearchCase::IgnoreCase, ESearchDir::FromStart, Start);
			if (End == INDEX_NONE) End = URL.Len();
			CurrentSessionID = URL.Mid(Start, End - Start);
		}
	}

	// MatchPlayers
	{
		const FString Key = TEXT("MatchPlayers=");
		int32 Index = URL.Find(Key, ESearchCase::IgnoreCase, ESearchDir::FromStart);
		if (Index != INDEX_NONE)
		{
			const int32 Start = Index + Key.Len();
			int32 End = URL.Find(TEXT("&"), ESearchCase::IgnoreCase, ESearchDir::FromStart, Start);
			if (End == INDEX_NONE) End = URL.Len();
			const FString Value = URL.Mid(Start, End - Start);
			ExpectedPlayerCount = FCString::Atoi(*Value);
		}
	}

	if (ExpectedPlayerCount <= 0)
	{
		ExpectedPlayerCount = 2; // 기본 안전값
	}
}

void AITBattleGameMode::StartMatchWhenReady()
{
	if (ExpectedPlayerCount <= 0) return;

	if (MatchPlayers.Num() >= ExpectedPlayerCount)
	{
		if (!HasMatchStarted())
		{
			StartMatch();
			UE_LOG(LogTemp, Log, TEXT("Match started. Session=%s Players=%d"), *CurrentSessionID, MatchPlayers.Num());
		}
	}
}
