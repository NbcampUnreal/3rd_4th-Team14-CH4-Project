#include "Network/ITLobbyGameMode.h"
#include "Network/ITLobbyGameState.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "Engine/Engine.h"
#include "HAL/PlatformProcess.h"
#include "Misc/Paths.h"
#include "Sockets.h"
#include "SocketSubsystem.h"

AITLobbyGameMode::AITLobbyGameMode()
{
	GameStateClass = AITLobbyGameState::StaticClass();
	bUseSeamlessTravel = false;
}

void AITLobbyGameMode::BeginPlay()
{
	Super::BeginPlay();
	GetWorld()->GetTimerManager().SetTimer(MainTimerHandle, this, &AITLobbyGameMode::OnMainTick, 1.f, true);
	GetWorld()->GetTimerManager().SetTimer(SessionCleanupTimerHandle, this, &AITLobbyGameMode::OnSessionCleanupTick, 5.f, true);
}

void AITLobbyGameMode::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().ClearTimer(MainTimerHandle);
		World->GetTimerManager().ClearTimer(SessionCleanupTimerHandle);
	}
	ShutdownAllProcesses();
}

void AITLobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	AliveControllers.Add(NewPlayer);
	UpdateAliveCount();
}

void AITLobbyGameMode::Logout(AController* Existing)
{
	Super::Logout(Existing);
	for (int32 i = AliveControllers.Num() - 1; i >= 0; --i)
	{
		if (!AliveControllers[i].IsValid() || AliveControllers[i].Get() == Existing)
		{
			AliveControllers.RemoveAt(i);
		}
	}
	UpdateAliveCount();
}

void AITLobbyGameMode::UpdateAliveCount()
{
	if (AITLobbyGameState* GS = GetLobbyGS())
	{
		AliveControllers.RemoveAll([](const TWeakObjectPtr<APlayerController>& It) { return !It.IsValid(); });
		GS->AlivePlayerControllerCount = AliveControllers.Num();
	}
}

AITLobbyGameState* AITLobbyGameMode::GetLobbyGS() const
{
	return GetGameState<AITLobbyGameState>();
}

bool AITLobbyGameMode::ShouldUseDedicatedPath() const
{
#if WITH_EDITOR
	return bForceDedicatedInEditor;
#else
	return true;
#endif
}

void AITLobbyGameMode::JoinMatchmakingQueue(APlayerController* Player)
{
	if (!IsValid(Player)) 
	{ 
		return; 
	}

	const bool bExists = MatchmakingQueue.ContainsByPredicate(
		[Player](const FMatchmakingPlayerInfo& It) { return It.PlayerController == Player; });
	if (bExists) 
	{
		return;
	}

	FMatchmakingPlayerInfo Info;
	Info.PlayerController = Player;
	Info.PlayerName = Player->GetName();
	Info.JoinTime = GetWorld()->GetTimeSeconds();
	MatchmakingQueue.Add(Info);

	if (AITLobbyGameState* GS = GetLobbyGS())
	{
		GS->MatchmakingPlayerCount = MatchmakingQueue.Num();
	}
}

void AITLobbyGameMode::LeaveMatchmakingQueue(APlayerController* Player)
{
	if (!IsValid(Player))
	{
		return;
	}

	MatchmakingQueue.RemoveAll([Player](const FMatchmakingPlayerInfo& It) { return It.PlayerController == Player; });

	if (AITLobbyGameState* GS = GetLobbyGS())
	{
		GS->MatchmakingPlayerCount = MatchmakingQueue.Num();
	}
}

void AITLobbyGameMode::OnMainTick()
{
	const int32 MinPlayersToStart = 3;

	for (int32 i = MatchmakingQueue.Num() - 1; i >= 0; --i)
	{
		if (!MatchmakingQueue[i].PlayerController.IsValid())
		{
			MatchmakingQueue.RemoveAt(i);
		}
	}

	if (MatchmakingQueue.Num() >= MinPlayersToStart)
	{
		TArray<FMatchmakingPlayerInfo> PlayersToMove;
		PlayersToMove.Reserve(MinPlayersToStart);
		for (int32 i = 0; i < MinPlayersToStart; ++i)
		{
			PlayersToMove.Add(MatchmakingQueue[i]);
		}
		MatchmakingQueue.RemoveAt(0, MinPlayersToStart, false);

		const FString SessionID = FString::Printf(TEXT("S_%lld"), FDateTime::UtcNow().GetTicks());
		StartGame(PlayersToMove, SessionID);
	}
}

void AITLobbyGameMode::OnSessionCleanupTick()
{
	const float Now = GetWorld()->GetTimeSeconds();

	for (int32 i = ActiveMatchProcesses.Num() - 1; i >= 0; --i)
	{
		FLaunchedMatchProcess& P = ActiveMatchProcesses[i];
		if (!P.bActive || !FPlatformProcess::IsProcRunning(P.ProcHandle))
		{
			SessionIdToConnectURL.Remove(P.SessionID);
			ShutdownProcess(P);
			ActiveMatchProcesses.RemoveAt(i);
			continue;
		}
		// 만약 세션 만료 로직이 있다면 여기에서 SessionIdToConnectURL.Remove(P.SessionID)도 같이 처리
	}
}

FString AITLobbyGameMode::BuildServerURL(const FString& MapName, const FString& SessionID, int32 ExpectedPlayers, int32 Port) const
{
	const FString Result = FString::Printf(TEXT("%s?SessionID=%s?MatchPlayers=%d -port=%d -server -log"),
		*MapName, *SessionID, ExpectedPlayers, Port);

	// 디버그 로그: 포트가 올바르게 전달되는지 확인
	UE_LOG(LogTemp, Warning, TEXT("BuildServerURL: Port=%d, Result=%s"), Port, *Result);

	return Result;
}

bool AITLobbyGameMode::CanConnectToPort(int32 Port, const FString& Host) const
{
	ISocketSubsystem* Subsystem = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM);
	if (!Subsystem)
	{
		return false;
	}

	TSharedRef<FInternetAddr> Addr = Subsystem->CreateInternetAddr();
	bool b = false;
	Addr->SetIp(*Host, b);
	Addr->SetPort(Port);

	FSocket* S = Subsystem->CreateSocket(NAME_Stream, TEXT("ConnectCheck"), false);
	if (!S) 
	{ 
		return false; 
	}
	S->SetNonBlocking(true);
	const bool bConn = S->Connect(*Addr);
	S->Close();
	Subsystem->DestroySocket(S);
	return bConn;
}

bool AITLobbyGameMode::GetAvailablePort(int32& OutPort)
{
	FScopeLock Lock(&PortAllocationMutex); // 동시 실행 방지

	// 현재 활성 프로세스들이 사용하는 포트 목록
	TSet<int32> UsedPorts;
	for (FLaunchedMatchProcess& Proc : ActiveMatchProcesses)
	{
		if (Proc.Port > 0)
		{
			UsedPorts.Add(Proc.Port);
		}
	}

	// 포트 카운터 방식으로 할당
	const int32 MaxTries = 100;
	for (int32 i = 0; i < MaxTries; ++i)
	{
		int32 CandidatePort = NextAvailablePort++;

		// 포트 범위 리셋 (너무 높아지면)
		if (NextAvailablePort > BaseServerPort + 1000)
		{
			NextAvailablePort = BaseServerPort;
		}

		// 이미 사용 중인 포트 스킵
		if (UsedPorts.Contains(CandidatePort))
		{
			continue;
		}

		OutPort = CandidatePort;
		UE_LOG(LogTemp, Warning, TEXT("Assigned port: %d"), OutPort);
		return true;
	}

	UE_LOG(LogTemp, Error, TEXT("Failed to find available port after %d tries"), MaxTries);
	return false;
}

bool AITLobbyGameMode::LaunchDedicatedServerProcess(const FString& SessionID, int32 ExpectedPlayers, int32& OutPort, FString& OutConnectURL, FLaunchedMatchProcess& OutProcess)
{
	OutPort = 0;
	OutConnectURL.Empty();

	if (!GetAvailablePort(OutPort))
	{
		UE_LOG(LogTemp, Error, TEXT("No available port from %d"), BaseServerPort);
		return false;
	}

	FLaunchedMatchProcess ReservedProcess(SessionID, OutPort, GetWorld()->GetTimeSeconds(), FProcHandle(), nullptr, nullptr);
	ReservedProcess.bActive = false; // 예약 상태
	ActiveMatchProcesses.Add(ReservedProcess);

	UE_LOG(LogTemp, Warning, TEXT("Reserved port %d for session %s"), OutPort, *SessionID);


	FString ServerExe = OverrideEditorServerExe;
	if (!FPaths::FileExists(ServerExe))
	{
		UE_LOG(LogTemp, Error, TEXT("Server exe not found: %s"), *ServerExe);
		// 예약 취소
		ActiveMatchProcesses.RemoveAll([&](const FLaunchedMatchProcess& P) { return P.SessionID == SessionID && !P.bActive; });
		return false;
	}

	const FString URLArgs = BuildServerURL(MatchMapName.ToString(), SessionID, ExpectedPlayers, OutPort);
	UE_LOG(LogTemp, Warning, TEXT("GetAvailablePort returned: %d"), OutPort);
	UE_LOG(LogTemp, Warning, TEXT("BuildServerURL: Port=%d, Result=%s"), OutPort, *URLArgs);
	UE_LOG(LogTemp, Warning, TEXT("CreateProc Args: %s %s"), *ServerExe, *URLArgs);

	void* ReadPipe = nullptr;
	void* WritePipe = nullptr;
	FPlatformProcess::CreatePipe(ReadPipe, WritePipe);

	const bool bLaunchDetached = true;
	const bool bLaunchHidden = false;
	const bool bLaunchReallyHidden = false;

	FProcHandle Handle = FPlatformProcess::CreateProc(*ServerExe, *URLArgs, bLaunchDetached, bLaunchHidden, bLaunchReallyHidden, nullptr, 0, nullptr, WritePipe);

	if (!Handle.IsValid())
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to launch DS: %s %s"), *ServerExe, *URLArgs);
		FPlatformProcess::ClosePipe(ReadPipe, WritePipe);
		// 예약 취소
		ActiveMatchProcesses.RemoveAll([&](const FLaunchedMatchProcess& P) { return P.SessionID == SessionID && !P.bActive; });
		return false;
	}

	// 예약을 실제 프로세스로 승격
	for (FLaunchedMatchProcess& P : ActiveMatchProcesses)
	{
		if (P.SessionID == SessionID && !P.bActive)
		{
			P.bActive = true;
			P.ProcHandle = Handle;
			P.ReadPipe = ReadPipe;
			P.WritePipe = WritePipe;
			OutProcess = P;
			break;
		}
	}

	const float StartTime = GetWorld()->GetTimeSeconds();
	//OutProcess = FLaunchedMatchProcess(SessionID, OutPort, StartTime, Handle, ReadPipe, WritePipe);
	//ActiveMatchProcesses.Add(OutProcess);

	bool bReady = false;
	int32 ActualPort = OutPort; // 실제 바인드된 포트를 찾기 위한 변수
	FString AccLog;

	while ((GetWorld()->GetTimeSeconds() - StartTime) < ServerBootTimeout)
	{
		FPlatformProcess::Sleep(0.1f);

		if (ReadPipe)
		{
			const FString Chunk = FPlatformProcess::ReadPipe(ReadPipe);
			if (!Chunk.IsEmpty())
			{
				AccLog += Chunk;

				// 실제 바인드된 포트를 로그에서 찾기
				FString ListenLog;
				if (AccLog.Contains(TEXT("IpNetDriver listening on port")))
				{
					int32 ListenStart = AccLog.Find(TEXT("IpNetDriver listening on port"));
					if (ListenStart != INDEX_NONE)
					{
						FString PortStr = AccLog.RightChop(ListenStart + 30); // "IpNetDriver listening on port " 길이
						int32 LineEnd = PortStr.Find(TEXT("\n"));
						if (LineEnd != INDEX_NONE)
						{
							PortStr = PortStr.Left(LineEnd);
						}
						ActualPort = FCString::Atoi(*PortStr.TrimStartAndEnd());
						if (ActualPort > 0)
						{
							UE_LOG(LogTemp, Warning, TEXT("DS actually listening on port: %d"), ActualPort);
							bReady = true;
							break;
						}
					}
				}

				if (AccLog.Contains(TEXT("Bringing World up for play")) || AccLog.Contains(TEXT("Game Engine Initialized")))
				{
					bReady = true;
					break;
				}
			}
		}

		if (!FPlatformProcess::IsProcRunning(Handle))
		{
			UE_LOG(LogTemp, Error, TEXT("DS exited prematurely: %s"), *SessionID);
			break;
		}
	}

	if (!bReady)
	{
		UE_LOG(LogTemp, Error, TEXT("Dedicated server not ready in time. Session %s Port %d"), *SessionID, OutPort);
		ShutdownProcessBySession(SessionID);
		return false;
	}

	// 실제 바인드된 포트를 사용
	OutPort = ActualPort;
	OutConnectURL = FString::Printf(TEXT("127.0.0.1:%d"), ActualPort);
	SessionIdToConnectURL.Add(SessionID, OutConnectURL);

	UE_LOG(LogTemp, Warning, TEXT("Dedicated server ready: %s @ %s"), *SessionID, *OutConnectURL);
	return true;
}

void AITLobbyGameMode::StartMatchmaking()
{
	OnMainTick();
}

void AITLobbyGameMode::StartGame(const TArray<FMatchmakingPlayerInfo>& PlayersToMove, const FString& NewSessionID)
{
	if (!ShouldUseDedicatedPath()) return;

	TArray<APlayerController*> Controllers;
	for (const auto& Info : PlayersToMove)
	{
		if (APlayerController* PC = Info.PlayerController.Get())
		{
			Controllers.Add(PC);
		}
	}
	if (Controllers.Num() == 0)
	{
		return;
	}

	int32 Port = 0;
	FString ConnectURL;
	FLaunchedMatchProcess Proc;

	const int32 ExpectedPlayers = Controllers.Num();
	// 중요: 항상 새 DS를 생성해 ConnectURL을 확보
	if (!LaunchDedicatedServerProcess(NewSessionID, ExpectedPlayers, Port, ConnectURL, Proc))
	{
		// 실패 시 큐 롤백
		for (APlayerController* PC : Controllers)
		{
			if (PC)
			{
				FMatchmakingPlayerInfo Back;
				Back.PlayerController = PC;
				Back.PlayerName = PC->GetName();
				Back.JoinTime = GetWorld()->GetTimeSeconds();
				MatchmakingQueue.Insert(Back, 0);
			}
		}
		return;
	}

	// 세션별 URL을 다시 확인(안전)
	if (const FString* CachedURL = SessionIdToConnectURL.Find(NewSessionID))
	{
		ConnectURL = *CachedURL;
	}

	for (APlayerController* PC : Controllers)
	{
		if (PC)
		{
			UE_LOG(LogTemp, Warning, TEXT("Travel Session=%s URL=%s PC=%s"), *NewSessionID, *ConnectURL, *PC->GetName());
			PC->ClientTravel(ConnectURL, TRAVEL_Absolute);
		}
	}
}

void AITLobbyGameMode::ShutdownProcess(FLaunchedMatchProcess& Proc)
{
	if (Proc.bActive)
	{
		if (FPlatformProcess::IsProcRunning(Proc.ProcHandle))
		{
			FPlatformProcess::TerminateProc(Proc.ProcHandle, true);
		}
		FPlatformProcess::CloseProc(Proc.ProcHandle);
		Proc.bActive = false;
	}
	if (Proc.ReadPipe || Proc.WritePipe)
	{
		FPlatformProcess::ClosePipe(Proc.ReadPipe, Proc.WritePipe);
		Proc.ReadPipe = Proc.WritePipe = nullptr;
	}
}

void AITLobbyGameMode::ShutdownProcessBySession(const FString& SessionID)
{
	for (int32 i = 0; i < ActiveMatchProcesses.Num(); ++i)
	{
		if (ActiveMatchProcesses[i].SessionID == SessionID)
		{
			ShutdownProcess(ActiveMatchProcesses[i]);
			ActiveMatchProcesses.RemoveAt(i);
			break;
		}
	}
	SessionIdToConnectURL.Remove(SessionID); // 캐시 제거
}

void AITLobbyGameMode::ShutdownAllProcesses()
{
	for (FLaunchedMatchProcess& P : ActiveMatchProcesses)
	{
		ShutdownProcess(P);
	}
	ActiveMatchProcesses.Empty();
}
