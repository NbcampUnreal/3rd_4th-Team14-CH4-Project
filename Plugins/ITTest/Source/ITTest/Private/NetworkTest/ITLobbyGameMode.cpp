#include "NetworkTest/ITLobbyGameMode.h"
#include "NetworkTest/ITLobbyGameState.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "Engine/Engine.h"

AITLobbyGameMode::AITLobbyGameMode()
{
	GameStateClass = AITLobbyGameState::StaticClass();
	bUseSeamlessTravel = false;
}

void AITLobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	AliveControllers.Add(NewPlayer);
	UpdateAliveCount();

	UE_LOG(LogTemp, Warning, TEXT("Player Join: %d"), AliveControllers.Num());
}

void AITLobbyGameMode::Logout(AController* Existing)
{
	Super::Logout(Existing);
	APlayerController* PC = Cast<APlayerController>(Existing);

	AliveControllers.Remove(PC);
	LeaveMatchmakingQueue(PC);
	UpdateAliveCount();

	UE_LOG(LogTemp, Warning, TEXT("Player Exit: %d Users"), AliveControllers.Num());
}

void AITLobbyGameMode::BeginPlay()
{
	Super::BeginPlay();
	RemainCountdown = CountdownTime;
	GetWorld()->GetTimerManager().SetTimer(MainTimerHandle, this, &AITLobbyGameMode::OnMainTick, 1.f, true);

	// 서버 실행시 자동으로 매칭 활성화
	StartMatchmaking();
}

void AITLobbyGameMode::StartMatchmaking()
{
	AITLobbyGameState* GS = GetGameState<AITLobbyGameState>();
	if (!GS)
	{
		return;
	}

	if (GS->MatchState == EMatchState::WaitingForPlayers)
	{
		GS->MatchState = EMatchState::InMatchmaking;
		MatchmakingStartTime = GetWorld()->GetTimeSeconds();

		UE_LOG(LogTemp, Warning, TEXT("Matchmaking System Activated! Players can now press Start button."));
	}
}

void AITLobbyGameMode::JoinMatchmakingQueue(APlayerController* Player)
{
	UE_LOG(LogTemp, Warning, TEXT("=== JoinMatchmakingQueue Debug ==="));
	UE_LOG(LogTemp, Warning, TEXT("Player: %s"), Player ? *Player->GetName() : TEXT("NULL"));

	if (!Player)
	{
		UE_LOG(LogTemp, Error, TEXT("Player is NULL!"));
		return;
	}

	AITLobbyGameState* GS = GetGameState<AITLobbyGameState>();
	UE_LOG(LogTemp, Warning, TEXT("GameState: %s"), GS ? TEXT("Valid") : TEXT("NULL"));

	bool bIsMatchmakingActive = GS && (GS->MatchState == EMatchState::InMatchmaking || GS->MatchState == EMatchState::CountingDown);
	UE_LOG(LogTemp, Warning, TEXT("IsMatchmakingActive: %s"), bIsMatchmakingActive ? TEXT("True") : TEXT("False"));

	if (!GS || !bIsMatchmakingActive)
	{
		UE_LOG(LogTemp, Warning, TEXT("Matchmaking is not activated!"));
		return;
	}

	for (const FMatchmakingPlayerInfo& QueuePlayer : MatchmakingQueue)
	{
		if (QueuePlayer.PlayerController == Player)
		{
			UE_LOG(LogTemp, Warning, TEXT("Player is already in the matching queue"));
			return;
		}
	}

	if (MatchmakingQueue.Num() >= MaximumPlayers)
	{
		UE_LOG(LogTemp, Warning, TEXT("Matchmaking queue is full! Please try again later."));
		return;
	}

	FMatchmakingPlayerInfo NewPlayerInfo;
	NewPlayerInfo.PlayerController = Player;
	NewPlayerInfo.PlayerName = Player->GetName();

	float NewPlayerJoinTime = GetWorld()->GetTimeSeconds();
	NewPlayerInfo.JoinTime = NewPlayerJoinTime;
	LastPlayerJoinTime = NewPlayerJoinTime;

	MatchmakingQueue.Add(NewPlayerInfo);
	UpdateMatchmakingState();

	UE_LOG(LogTemp, Warning, TEXT("Player '%s' joined matchmaking queue! (%d/%d players)"),
		*NewPlayerInfo.PlayerName, MatchmakingQueue.Num(), MaximumPlayers);
}

void AITLobbyGameMode::LeaveMatchmakingQueue(APlayerController* Player)
{
	if (!Player)
	{
		return;
	}

	for (int32 i = MatchmakingQueue.Num() - 1; i >= 0; i--)
	{
		if (MatchmakingQueue[i].PlayerController == Player)
		{
			UE_LOG(LogTemp, Warning, TEXT("Player '%s' left the matchmaking queue"),
				*MatchmakingQueue[i].PlayerName);
			MatchmakingQueue.RemoveAt(i);
			break;
		}
	}

	UpdateMatchmakingState();
}

void AITLobbyGameMode::UpdateAliveCount()
{
	AITLobbyGameState* GS = GetGameState<AITLobbyGameState>();
	if (!GS)
	{
		return;
	}

	GS->AlivePlayerControllerCount = AliveControllers.Num();
}

void AITLobbyGameMode::UpdateMatchmakingState()
{
	AITLobbyGameState* GS = GetGameState<AITLobbyGameState>();
	if (!GS)
	{
		return;
	}

	GS->MatchmakingPlayerCount = MatchmakingQueue.Num();
}

void AITLobbyGameMode::OnMainTick()
{
	AITLobbyGameState* GS = GetGameState<AITLobbyGameState>();
	if (!GS)
	{
		return;
	}

	switch (GS->MatchState)
	{
	case EMatchState::InMatchmaking:
		CheckMatchmakingConditions();
		break;

	case EMatchState::CountingDown:
		--RemainCountdown;
		GS->CountdownTime = RemainCountdown;

		if (RemainCountdown <= 0)
		{
			StartGame();
		}
		UE_LOG(LogTemp, Warning, TEXT("Game starting in: %d seconds"), RemainCountdown);
		break;

	default:
		break;
	}
}

void AITLobbyGameMode::CheckMatchmakingConditions()
{
	AITLobbyGameState* GS = GetGameState<AITLobbyGameState>();
	if (!GS)
	{
		return;
	}

	float CurrentTime = GetWorld()->GetTimeSeconds();
	int32 QueueSize = MatchmakingQueue.Num();

	// 조건 1: 최대 인원 달성
	if (QueueSize >= MaximumPlayers)
	{
		UE_LOG(LogTemp, Warning, TEXT("Maximum players reached! Game starting soon"));
		GS->MatchState = EMatchState::CountingDown;
		RemainCountdown = CountdownTime;
		return;
	}

	// 조건 2: 최소 인원 + 새 플레이어 없음 타임아웃
	if (QueueSize >= MinimumPlayers)
	{
		float TimeSinceLastJoin = CurrentTime - LastPlayerJoinTime;
		if (TimeSinceLastJoin >= NoNewPlayerTimeout)
		{
			UE_LOG(LogTemp, Warning, TEXT("Minimum players met + no new players timeout! Starting game"));
			GS->MatchState = EMatchState::CountingDown;
			RemainCountdown = CountdownTime;
			return;
		}
	}

	// 조건 3: 매칭 전체 타임아웃
	float TotalMatchmakingTime = CurrentTime - MatchmakingStartTime;
	if (TotalMatchmakingTime >= MatchmakingTimeout)
	{
		if (QueueSize >= MinimumPlayers)
		{
			UE_LOG(LogTemp, Warning, TEXT("Matchmaking timeout! Starting game with current players"));
			GS->MatchState = EMatchState::CountingDown;
			RemainCountdown = CountdownTime;
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Matchmaking timeout + insufficient players! Restarting matchmaking"));
			ResetMatchmaking();
		}
		return;
	}
}

void AITLobbyGameMode::StartGame()
{
	AITLobbyGameState* GS = GetGameState<AITLobbyGameState>();
	if (!GS)
	{
		return;
	}

	GS->MatchState = EMatchState::Playing;

	UE_LOG(LogTemp, Warning, TEXT("Game Started! %d players participating"), MatchmakingQueue.Num());
	GetWorld()->ServerTravel(GameMapPath.ToString());
}

void AITLobbyGameMode::ResetMatchmaking()
{
	AITLobbyGameState* GS = GetGameState<AITLobbyGameState>();
	if (!GS)
	{
		return;
	}

	GS->MatchState = EMatchState::InMatchmaking;  // 다시 매칭 상태로
	GS->MatchmakingPlayerCount = MatchmakingQueue.Num();
	GS->CountdownTime = 0;

	// 대기열은 유지하고 타이머만 재시작
	RemainCountdown = CountdownTime;
	MatchmakingStartTime = GetWorld()->GetTimeSeconds();

	UE_LOG(LogTemp, Warning, TEXT("Matchmaking restarted. Waiting for more players..."));
}