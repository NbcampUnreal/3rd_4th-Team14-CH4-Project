#include "NetworkTest/ITLobbyGameState.h"
#include "Engine/Engine.h"
#include "Net/UnrealNetwork.h"

void AITLobbyGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AITLobbyGameState, AlivePlayerControllerCount);
	DOREPLIFETIME(AITLobbyGameState, MatchState);
	DOREPLIFETIME(AITLobbyGameState, MatchmakingPlayerCount);
	DOREPLIFETIME(AITLobbyGameState, CountdownTime);
}