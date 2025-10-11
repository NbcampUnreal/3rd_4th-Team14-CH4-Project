#include "GameModes/ITBattleGameMode.h"
#include "Engine/World.h"
#include "Engine/Engine.h"
#include "Player/ITPlayerState.h"
#include "Player/ITBattlePlayerController.h"

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
	if (AITPlayerState* PlayerState = Cast<AITPlayerState>(NewPlayer->PlayerState))
	{
		AlivePlayers.AddUnique(PlayerState);
		UE_LOG(LogTemp, Log, TEXT("Player joined and added to alive list: %s"), *PlayerState->GetPlayerName());
	}

	StartMatchWhenReady();
}

void AITBattleGameMode::RestartPlayer(AController* NewPlayer)
{
	Super::RestartPlayer(NewPlayer);
}

void AITBattleGameMode::OnPlayerDeath(AITPlayerState* DeadPlayer)
{
	if (!DeadPlayer || bGameEnded) return;

	UE_LOG(LogTemp, Warning, TEXT("Player died: %s"), *DeadPlayer->GetPlayerName());

	// 생존자 배열에서 제거
	AlivePlayers.Remove(DeadPlayer);

	UE_LOG(LogTemp, Warning, TEXT("Alive players: %d"), AlivePlayers.Num());

	// 승자 확인
	CheckForWinner();
}

void AITBattleGameMode::CheckForWinner()
{
	if (bGameEnded) return;

	// 생존자가 1명 이하일 때 게임 종료
	if (AlivePlayers.Num() <= 1)
	{
		AITPlayerState* Winner = (AlivePlayers.Num() == 1) ? AlivePlayers[0] : nullptr;
		EndGame(Winner);
	}
}

void AITBattleGameMode::EndGame(AITPlayerState* Winner)
{
	if (bGameEnded) 
	{ 
		return; 
	}

	bGameEnded = true;

	if (Winner)
	{
		UE_LOG(LogTemp, Warning, TEXT("Game Over! Winner: %s"), *Winner->GetPlayerName());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Game Over! No winner (Draw)"));
	}

	EndMatch();

	// 모든 플레이어에게 결과창 표시
	ShowResultToAllPlayers(Winner);

	FTimerHandle ShutdownTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(ShutdownTimerHandle, [this]()
		{
			UE_LOG(LogTemp, Warning, TEXT("Match ended - Server shutting down"));
			// 배틀 서버는 일회성이므로 게임 종료 후 서버를 종료할 수 있음
#if !WITH_EDITOR
			FGenericPlatformMisc::RequestExit(false);
#endif
		}, 30.0f, false); // 30초 후 서버 종료
}

void AITBattleGameMode::ShowResultToAllPlayers(AITPlayerState* Winner)
{
	for (APlayerController* PC : MatchPlayers)
	{
		if (AITBattlePlayerController* BattlePC = Cast<AITBattlePlayerController>(PC))
		{
			FString WinnerName = Winner ? Winner->GetPlayerName() : TEXT("No Winner");
			BattlePC->ClientShowResult(WinnerName);
		}
	}
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
