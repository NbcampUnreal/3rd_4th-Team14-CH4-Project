#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "Network/ITMatchTypes.h"
#include "ITLobbyGameState.generated.h"

UCLASS()
class IMITATIONTRIGGER_API AITLobbyGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly)
	int32 AlivePlayerControllerCount = 0;

	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly)
	EMatchState MatchState = EMatchState::WaitingForPlayers;

	// 매칭 대기열 관련
	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly)
	int32 MatchmakingPlayerCount = 0;

	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly)
	int32 CountdownTime = 0;
	// 활성화된 매치 세션들
	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly)
	TArray<FActiveMatchSession> ActiveMatchSessions;

	// 총 진행 중인 매치 수
	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly)
	int32 ActiveMatchCount = 0;


	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& Out) const override;
};
