#pragma once
#include "CoreMinimal.h"
#include "ITMatchTypes.generated.h"

UENUM(BlueprintType)
enum class EMatchState : uint8
{
	None,
	WaitingForPlayers,    // 플레이어 대기
	InMatchmaking,        // 매칭 진행 중
	CountingDown,         // 게임 시작 카운트다운
	Playing,              // 게임 진행 중
	Ending,
	End
};

// 매칭 대기열 플레이어 정보
USTRUCT(BlueprintType)
struct FMatchmakingPlayerInfo
{
	GENERATED_BODY()

	UPROPERTY()
	TWeakObjectPtr<APlayerController> PlayerController;

	UPROPERTY()
	FString PlayerName;

	UPROPERTY()
	float JoinTime;  // 대기열 입장 시간

	FMatchmakingPlayerInfo()
	{
		PlayerController = nullptr;
		PlayerName = TEXT("");
		JoinTime = 0.0f;
	}
};

// 활성화된 매치 정보 (Session 기반)
USTRUCT(BlueprintType)
struct FActiveMatchSession
{
	GENERATED_BODY()

	UPROPERTY()
	FString SessionID; // 고유 세션 ID

	UPROPERTY()
	TArray<FMatchmakingPlayerInfo> Players;  // 이 매치의 플레이어들

	UPROPERTY()
	float StartTime; // 매치 시작 시간

	UPROPERTY()
	EMatchState CurrentState; // 현재 매치 상태

	UPROPERTY()
	bool bIsActive; // 매치 활성화 여부

	FActiveMatchSession()
	{
		SessionID = TEXT("");
		Players.Empty();
		StartTime = 0.0f;
		CurrentState = EMatchState::Playing;
		bIsActive = true;
	}
};
