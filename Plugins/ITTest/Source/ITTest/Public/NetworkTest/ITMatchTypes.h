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
struct FMatchmakingPlayer
{
	GENERATED_BODY()

	UPROPERTY()
	TWeakObjectPtr<APlayerController> PlayerController;

	UPROPERTY()
	FString PlayerName;

	UPROPERTY()
	float JoinTime;  // 대기열 입장 시간

	FMatchmakingPlayer()
	{
		PlayerController = nullptr;
		PlayerName = TEXT("");
		JoinTime = 0.0f;
	}
};