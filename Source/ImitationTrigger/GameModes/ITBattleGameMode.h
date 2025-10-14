#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "Network/ITMatchTypes.h"
#include "ITBattleGameMode.generated.h"

class AITPlayerState;

UCLASS()
class IMITATIONTRIGGER_API AITBattleGameMode : public AGameMode
{
	GENERATED_BODY()
	
public:
	AITBattleGameMode();

	virtual void BeginPlay() override;
	virtual void EndPlay(EEndPlayReason::Type EndPlayReason) override;
	virtual void PostLogin(APlayerController* NewPlayer) override;
	virtual void RestartPlayer(AController* NewPlayer) override;
	virtual void Logout(AController* Exiting) override;

	UFUNCTION(BlueprintCallable)
	void ReturnToLobby();

	// 플레이어 사망 처리
	void OnPlayerDeath(AITPlayerState* DeadPlayer);

	const TArray<TObjectPtr<APlayerController>>& GetMatchPlayers() const { return MatchPlayers; }

	int32 GetAlivePlayerCount() const { return AlivePlayers.Num(); }

protected:
	// 로비에서 URL로 넘겨주는 값
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Match")
	FString CurrentSessionID;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Match")
	int32 ExpectedPlayerCount = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Match")
	TArray<TObjectPtr<APlayerController>> MatchPlayers;

	// 생존자 배열
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Match")
	TArray<TObjectPtr<AITPlayerState>> AlivePlayers;

	// 게임 종료 여부
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Match")
	bool bGameEnded = false;

	// 승자 확인 및 게임 종료 처리
	void CheckForWinner();
	void EndGame(AITPlayerState* Winner);

	// 모든 클라이언트에게 결과창 표시 요청
	void ShowResultToAllPlayers(AITPlayerState* Winner);

	void ExtractSessionInfoFromURL(); // ThirdPersonMap?SessionID=...&MatchPlayers=...
	void StartMatchWhenReady();

	void UpdateAlivePlayerCountToAllPlayers();

private:
	FTimerHandle ShowResultTimerHandle;
	FTimerHandle ShutdownTimerHandle;
};
