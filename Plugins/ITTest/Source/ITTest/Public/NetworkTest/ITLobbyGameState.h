#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "NetworkTest/ITMatchTypes.h"
#include "ITLobbyGameState.generated.h"



UCLASS()
class ITTEST_API AITLobbyGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly)
	int32 AlivePlayerControllerCount = 0;

	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly)
	EMatchState MatchState = EMatchState::WaitingForPlayers;

	// ��Ī ��⿭ ����
	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly)
	int32 MatchmakingPlayerCount = 0;

	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly)
	int32 CountdownTime = 0;

	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly)
	bool bMatchmakingActive = false;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& Out) const override;
};
