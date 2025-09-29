#include "ITLobbyGameMode.h"
#include "Engine/World.h"
#include "Engine/Engine.h"
#include "Misc/CommandLine.h"
#include "Misc/Paths.h"
#include "HAL/PlatformProcess.h"
#include "Kismet/GameplayStatics.h"

AITLobbyGameMode::AITLobbyGameMode()
{
	bUseSeamlessTravel = false; // 간단화
}

void AITLobbyGameMode::BeginPlay()
{
	Super::BeginPlay();

	// 명령행으로 ExternalHost 오버라이드: -ExternalHost=127.0.0.1
	// 1. ExternalHost 설정 (IP 주소)
	FString CmdHost;
	if (FParse::Value(FCommandLine::Get(), TEXT("ExternalHost="), CmdHost))
	{
		ExternalHost = CmdHost;
		UE_LOG(LogTemp, Warning, TEXT("ExternalHost set from command line: %s"), *ExternalHost);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Using default ExternalHost: %s"), *ExternalHost);
	}

	// 2. MinPlayersToStart 설정 (최소 시작 인원)
	FString CmdMinPlayers;
	if (FParse::Value(FCommandLine::Get(), TEXT("MinPlayers="), CmdMinPlayers))
	{
		int32 ParsedMinPlayers = FCString::Atoi(*CmdMinPlayers);
		if (ParsedMinPlayers > 0 && ParsedMinPlayers <= 100) // 유효성 검증
		{
			MinPlayersToStart = ParsedMinPlayers;
			UE_LOG(LogTemp, Warning, TEXT("MinPlayersToStart set from command line: %d"), MinPlayersToStart);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Invalid MinPlayers value: %s (using default: %d)"), *CmdMinPlayers, MinPlayersToStart);
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Using default MinPlayersToStart: %d"), MinPlayersToStart);
	}

	// 3. ServerExePath 설정 (서버 실행 파일 경로)
	FString CmdServerPath;
	if (FParse::Value(FCommandLine::Get(), TEXT("ServerExePath="), CmdServerPath))
	{
		if (FPaths::FileExists(CmdServerPath))
		{
			OverrideEditorServerExe = CmdServerPath;
			UE_LOG(LogTemp, Warning, TEXT("ServerExePath set from command line: %s"), *OverrideEditorServerExe);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("ServerExePath file not found: %s (using default)"), *CmdServerPath);
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Using default ServerExePath: %s"), *OverrideEditorServerExe);
	}

	// 4. BaseServerPort 설정 (선택사항) 타이틀에서 7777포트 입력해서 로비 진입
	FString CmdBasePort;
	if (FParse::Value(FCommandLine::Get(), TEXT("BasePort="), CmdBasePort))
	{
		int32 ParsedPort = FCString::Atoi(*CmdBasePort);
		if (ParsedPort >= 1024 && ParsedPort <= 65535) // 유효성 검증
		{
			BaseServerPort = ParsedPort;
			UE_LOG(LogTemp, Warning, TEXT("BaseServerPort set from command line: %d"), BaseServerPort);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Invalid BasePort value: %s (using default: %d)"), *CmdBasePort, BaseServerPort);
		}
	}

	NextPort = BaseServerPort; // 7778부터 사용

	// ⭐ 환경별 설정 요약 출력
#if WITH_EDITOR
	UE_LOG(LogTemp, Warning, TEXT("=== EDITOR MODE DETECTED ==="));
	UE_LOG(LogTemp, Warning, TEXT("Editor mode: Matchmaking will directly travel to game map"));
	UE_LOG(LogTemp, Warning, TEXT("No dedicated server process will be launched"));
	UE_LOG(LogTemp, Warning, TEXT("ExternalHost: %s (ignored in editor)"), *ExternalHost);
	UE_LOG(LogTemp, Warning, TEXT("MinPlayersToStart: %d (ignored in editor)"), MinPlayersToStart);
	UE_LOG(LogTemp, Warning, TEXT("============================"));
#else
	UE_LOG(LogTemp, Warning, TEXT("=== SERVER CONFIGURATION ==="));
	UE_LOG(LogTemp, Warning, TEXT("ExternalHost: %s"), *ExternalHost);
	UE_LOG(LogTemp, Warning, TEXT("MinPlayersToStart: %d"), MinPlayersToStart);
	UE_LOG(LogTemp, Warning, TEXT("BaseServerPort: %d"), BaseServerPort);
	UE_LOG(LogTemp, Warning, TEXT("ServerExePath: %s"), *OverrideEditorServerExe);
	UE_LOG(LogTemp, Warning, TEXT("============================"));
#endif
}

void AITLobbyGameMode::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

void AITLobbyGameMode::JoinMatchmakingQueue(APlayerController* PC)
{
	if (!PC) return;

#if WITH_EDITOR
	// ⭐ 에디터 환경: 즉시 게임 맵으로 이동 (매칭 없음)
	UE_LOG(LogTemp, Warning, TEXT("EDITOR MODE: Directly traveling to %s"), *MatchMap);
	// PC->ClientTravel(MatchMap, TRAVEL_Absolute); // 바로 ThirdPersonMap으로 이동
	GetWorld()->ServerTravel(MatchMap); //에디터 테스트 시에는 서버 트래블로 로컬에 join한 모든 클라 한번에 이동.
	return;
#else
	// ⭐ 패키징 환경: 기존 매칭 로직 실행
	MatchmakingQueue.AddUnique(PC);

	UE_LOG(LogTemp, Warning, TEXT("Player joined queue: %s (%d/%d)"),
		*PC->GetName(), MatchmakingQueue.Num(), MinPlayersToStart);

	// 자동 매칭 체크 추가
	if (MatchmakingQueue.Num() >= MinPlayersToStart)
	{
		UE_LOG(LogTemp, Warning, TEXT("Starting matchmaking automatically"));
		StartMatchmaking();
	}
#endif
}

void AITLobbyGameMode::LeaveMatchmakingQueue(APlayerController* PC)
{
	if (!PC) return;

#if !WITH_EDITOR
	// 패키징 환경에서만 실행
	MatchmakingQueue.Remove(PC);
#endif
}

int32 AITLobbyGameMode::GetNextPort()
{
	return NextPort++; // 매우 단순한 증가형
}

FString AITLobbyGameMode::BuildServerURLArgs(const FString& SessionID, int32 ExpectedPlayers) const
{
#if WITH_EDITOR
	// 에디터에서는 호출되지 않아야 하지만 안전장치
	return TEXT("");
#else
	// ThirdPersonMap?SessionID=abcd&MatchPlayers=4
	return FString::Printf(TEXT("%s?SessionID=%s&MatchPlayers=%d"), *MatchMap, *SessionID, ExpectedPlayers);
#endif
}

bool AITLobbyGameMode::ResolveServerExecutable(FString& OutServerExe) const
{
#if WITH_EDITOR
	// 에디터에서는 호출되지 않아야 하지만 안전장치
	UE_LOG(LogTemp, Warning, TEXT("ResolveServerExecutable should not be called in editor mode"));
	return false;
#else
	// 1) 에디터/로컬 전용 오버라이드 경로가 지정되어 있으면 최우선
	if (!OverrideEditorServerExe.IsEmpty() && FPaths::FileExists(OverrideEditorServerExe))
	{
		OutServerExe = OverrideEditorServerExe;
		return true;
	}

	// 2) 패키징된 서버 기본 경로(플랫폼별)
#if PLATFORM_WINDOWS
	const FString Packaged = FPaths::ConvertRelativePathToFull(FPaths::ProjectDir() / DefaultPackagedServerExeWin64);
#else
	const FString Packaged = FPaths::ConvertRelativePathToFull(FPaths::ProjectDir() / DefaultPackagedServerExeLinux);
#endif
	if (FPaths::FileExists(Packaged))
	{
		OutServerExe = Packaged;
		return true;
	}

	// 3) 실패
	return false;
#endif
}

bool AITLobbyGameMode::LaunchDedicatedServer(const FString& SessionID, int32 ExpectedPlayers, int32& OutPort, FString& OutConnectURL)
{
#if WITH_EDITOR
	// 에디터 모드에서는 호출되지 않아야 함
	UE_LOG(LogTemp, Error, TEXT("LaunchDedicatedServer should not be called in editor mode"));
	return false;
#else
	OutPort = GetNextPort();

	FString ServerExe;
	if (!ResolveServerExecutable(ServerExe))
	{
		UE_LOG(LogTemp, Error, TEXT("Server exe not found. Check OverrideEditorServerExe or packaged path."));
		return false;
	}

	const FString URLArgs = BuildServerURLArgs(SessionID, ExpectedPlayers);
	// -server -log -Port=NNNN 만 사용(파이프/로그 파싱 제거)
	const FString ProcArgs = FString::Printf(TEXT("%s -server -log -Port=%d -unattended -nopause"), *URLArgs, OutPort);

	UE_LOG(LogTemp, Warning, TEXT("Launching dedicated server: %s %s"), *ServerExe, *ProcArgs);

	FProcHandle Handle = FPlatformProcess::CreateProc(
		*ServerExe,
		*ProcArgs,
		true,   // detached
		false,
		false,
		nullptr,
		0,
		*FPaths::ProjectDir(),
		nullptr
	);

	if (!Handle.IsValid())
	{
		UE_LOG(LogTemp, Error, TEXT("CreateProc failed: %s %s"), *ServerExe, *ProcArgs);
		return false;
	}

	// 외부 접속 주소 구성: 로비는 7777, 매치 DS는 7778+
	OutConnectURL = FString::Printf(TEXT("%s:%d"), *ExternalHost, OutPort); // 절대 주소
	return true;
#endif
}

void AITLobbyGameMode::StartMatchmaking()
{
#if WITH_EDITOR
	// 에디터 모드에서는 이 함수가 호출되지 않아야 함
	UE_LOG(LogTemp, Warning, TEXT("EDITOR MODE: StartMatchmaking should not be called"));
	return;
#else
	// 최소 인원 미달 시 대기
	if (MatchmakingQueue.Num() < MinPlayersToStart)
	{
		UE_LOG(LogTemp, Log, TEXT("Waiting for players: %d/%d"), MatchmakingQueue.Num(), MinPlayersToStart);
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("Starting matchmaking with %d players"), MatchmakingQueue.Num());

	// 큐에서 참가자 확정(전원 매칭)
	TArray<APlayerController*> MatchedControllers;
	for (const TWeakObjectPtr<APlayerController>& WeakPC : MatchmakingQueue)
	{
		if (APlayerController* PC = WeakPC.Get())
		{
			MatchedControllers.Add(PC);
		}
	}
	MatchmakingQueue.Reset(); // 큐 비우기

	if (MatchedControllers.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("No valid players in queue"));
		return;
	}

	// 세션 ID 생성
	const FString SessionID = FGuid::NewGuid().ToString(EGuidFormats::Short);
	const int32 ExpectedPlayers = MatchedControllers.Num();

	// 전용 서버 실행
	int32 Port = 0;
	FString ConnectURL;
	if (!LaunchDedicatedServer(SessionID, ExpectedPlayers, Port, ConnectURL))
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to launch DS for session %s"), *SessionID);
		// 실패 시 참가자들을 큐에 복귀
		for (APlayerController* PC : MatchedControllers)
		{
			MatchmakingQueue.Add(PC);
		}
		return;
	}

	// 모든 참가자를 전용 서버로 이동
	for (APlayerController* PC : MatchedControllers)
	{
		if (PC)
		{
			PC->ClientTravel(ConnectURL, TRAVEL_Absolute); // 예: "127.0.0.1:7778"
		}
	}

	UE_LOG(LogTemp, Log, TEXT("Launched DS %s at %s for %d players"), *SessionID, *ConnectURL, ExpectedPlayers);
#endif
}
