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

	// ��Ī �ý���
	UFUNCTION(BlueprintCallable)
	void StartMatchmaking();

	UFUNCTION(BlueprintCallable)
	void JoinMatchmakingQueue(APlayerController* Player);

	UFUNCTION(BlueprintCallable)
	void LeaveMatchmakingQueue(APlayerController* Player);

protected:
	// ��ü �÷��̾�
	UPROPERTY()
	TArray<TWeakObjectPtr<APlayerController>> AliveControllers;

	// ��Ī ��⿭ �÷��̾��
	UPROPERTY()
	TArray<FMatchmakingPlayer> MatchmakingQueue;

	FTimerHandle MainTimerHandle;
	FTimerHandle MatchmakingTimerHandle;

	// ��Ī ����
	UPROPERTY(EditDefaultsOnly, Category = "Matchmaking")
	int32 MinimumPlayers = 3;

	UPROPERTY(EditDefaultsOnly, Category = "Matchmaking")
	int32 MaximumPlayers = 10;

	UPROPERTY(EditDefaultsOnly, Category = "Matchmaking")
	float MatchmakingTimeout = 30.0f;  // ��Ī Ÿ�Ӿƿ� (30��)

	UPROPERTY(EditDefaultsOnly, Category = "Matchmaking")
	float CountdownTime = 5.0f;  // ���� ���� ī��Ʈ�ٿ�

	UPROPERTY(EditDefaultsOnly, Category = "Matchmaking")
	float NoNewPlayerTimeout = 10.0f;  // �� �÷��̾� ���� �� ��� �ð�

	// ���� ����
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
