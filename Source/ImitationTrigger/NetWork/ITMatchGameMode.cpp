#include "Network/ITMatchGameMode.h"
#include "Engine/World.h"
#include "Engine/Engine.h"
#include "GameFramework/GameModeBase.h"

AITMatchGameMode::AITMatchGameMode()
{
	bUseSeamlessTravel = false;
}

void AITMatchGameMode::BeginPlay()
{
	Super::BeginPlay();
	ExtractSessionInfoFromURL();

	UE_LOG(LogTemp, Warning, TEXT("Match started for Session: %s, expecting %d players"),
		*CurrentSessionID, ExpectedPlayerCount);
}

void AITMatchGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	MatchPlayers.Add(NewPlayer);

	UE_LOG(LogTemp, Warning, TEXT("Player joined match: %s (%d/%d)"),
		*NewPlayer->GetName(), MatchPlayers.Num(), ExpectedPlayerCount);

	// 모든 예상 플레이어가 도착했는지 확인
	if (MatchPlayers.Num() >= ExpectedPlayerCount)
	{
		StartMatchWhenReady();
	}
}

void AITMatchGameMode::ExtractSessionInfoFromURL()
{
	FString URL = GetWorld()->URL.ToString();

	// SessionID 추출
	FString SessionIDParam = TEXT("SessionID=");
	int32 SessionIDIndex = URL.Find(SessionIDParam);
	if (SessionIDIndex != INDEX_NONE)
	{
		int32 StartIndex = SessionIDIndex + SessionIDParam.Len();
		int32 EndIndex = URL.Find(TEXT("&"), StartIndex);
		if (EndIndex == INDEX_NONE)
		{
			EndIndex = URL.Len();
		}
		CurrentSessionID = URL.Mid(StartIndex, EndIndex - StartIndex);
	}

	// MatchPlayers 추출
	FString PlayersParam = TEXT("MatchPlayers=");
	int32 PlayersIndex = URL.Find(PlayersParam);
	if (PlayersIndex != INDEX_NONE)
	{
		int32 StartIndex = PlayersIndex + PlayersParam.Len();
		int32 EndIndex = URL.Find(TEXT("&"), StartIndex);
		if (EndIndex == INDEX_NONE)
		{
			EndIndex = URL.Len();
		}
		FString PlayersStr = URL.Mid(StartIndex, EndIndex - StartIndex);
		ExpectedPlayerCount = FCString::Atoi(*PlayersStr);
	}
}

void AITMatchGameMode::StartMatchWhenReady()
{
	UE_LOG(LogTemp, Warning, TEXT("Match Session %s is ready! %d players connected"),
		*CurrentSessionID, MatchPlayers.Num());

	// 여기서 실제 게임 로직 시작
	// 예: 스폰 포인트 배정, 게임 타이머 시작 등
}

