#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "NetworkTest/ITMatchTypes.h"
#include "ITLobbyGameMode.generated.h"

class AITLobbyGameState;

UCLASS()
class ITTEST_API AITLobbyGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AITLobbyGameMode();

	virtual void PostLogin(APlayerController* NewPlayer) override;
	virtual void Logout(AController* Existing) override;
	virtual void BeginPlay() override;

	// 매칭 시스템
	UFUNCTION(BlueprintCallable)
	void StartMatchmaking();

	UFUNCTION(BlueprintCallable)
	void JoinMatchmakingQueue(APlayerController* Player);

	UFUNCTION(BlueprintCallable)
	void LeaveMatchmakingQueue(APlayerController* Player);

protected:
	// 전체 플레이어
	UPROPERTY()
	TArray<TWeakObjectPtr<APlayerController>> AliveControllers;

	// 매칭 대기열 플레이어들
	UPROPERTY()
	TArray<FMatchmakingPlayer> MatchmakingQueue;

	FTimerHandle MainTimerHandle;
	FTimerHandle MatchmakingTimerHandle;

	// 매칭 설정
	UPROPERTY(EditDefaultsOnly, Category = "Matchmaking")
	int32 MinimumPlayers = 3;

	UPROPERTY(EditDefaultsOnly, Category = "Matchmaking")
	int32 MaximumPlayers = 10;

	UPROPERTY(EditDefaultsOnly, Category = "Matchmaking")
	float MatchmakingTimeout = 30.0f;  // 매칭 타임아웃 (30초)

	UPROPERTY(EditDefaultsOnly, Category = "Matchmaking")
	float CountdownTime = 5.0f;  // 게임 시작 카운트다운

	UPROPERTY(EditDefaultsOnly, Category = "Matchmaking")
	float NoNewPlayerTimeout = 10.0f;  // 새 플레이어 없을 때 대기 시간

	// 내부 상태
	float LastPlayerJoinTime = 0.0f;
	float MatchmakingStartTime = 0.0f;
	int32 RemainCountdown = 0;

	UPROPERTY(EditDefaultsOnly, Category = "Match")
	FName GameMapPath = FName(TEXT("/Game/ImitationTrigger/Network/Test/Third/ThirdPerson/Maps/ThirdPersonMap"));

	UFUNCTION()
	void OnMainTick();

	void UpdateAliveCount();
	void UpdateMatchmakingState();
	void CheckMatchmakingConditions();
	void StartGame();
	void ResetMatchmaking();
};
