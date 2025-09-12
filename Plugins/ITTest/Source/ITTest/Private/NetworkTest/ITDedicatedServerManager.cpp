#include "NetworkTest/ITDedicatedServerManager.h"
#include "Engine/World.h"
#include "Engine/Engine.h"
#include "Misc/Guid.h"
#include "Misc/DateTime.h"
#include "HAL/PlatformProcess.h"
#include "Misc/Paths.h"

void UITDedicatedServerManager::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);
    ITActiveSessions.Empty();
    UE_LOG(LogTemp, Warning, TEXT("IT Dedicated Server Manager Initialized"));
}

FString UITDedicatedServerManager::ITCreateGameSession(const TArray<FITPlayerMatchInfo>& Players, EITMatchmakingMode Mode)
{
    const FString SessionID = ITGenerateSessionID();

    FITGameSessionInfo SessionInfo;
    SessionInfo.SessionID = SessionID;
    SessionInfo.GameMode = Mode;
    SessionInfo.CreatedTime = FDateTime::Now();
    SessionInfo.ServerPort = ITGetPortForSession();
    SessionInfo.ServerIP = TEXT("127.0.0.1");

    for (const FITPlayerMatchInfo& Player : Players)
    {
        SessionInfo.PlayerIDs.Add(Player.PlayerID);
    }

    // 세션 저장 후 Spawn (Spawn에서 보정 가능)
    ITActiveSessions.Add(SessionID, SessionInfo);

    // 실제 데디 서버 프로세스 스폰
    if (ITSpawnDedicatedServer(SessionID, Mode))
    {
        // 로비/오케스트레이터 월드에서 ServerTravel은 절대 호출하지 않음
        ITNotifyPlayersOfGameStart(Players, ITActiveSessions[SessionID]);

        UE_LOG(LogTemp, Warning, TEXT("Game session created successfully: %s with %d players"),
            *SessionID, Players.Num());
        return SessionID;
    }

    // 실패 시 롤백
    ITActiveSessions.Remove(SessionID);
    UE_LOG(LogTemp, Error, TEXT("Failed to create dedicated server for session: %s"), *SessionID);
    return FString();
}

bool UITDedicatedServerManager::ITGetSessionInfo(const FString& SessionID, FITGameSessionInfo& OutInfo)
{
    if (const FITGameSessionInfo* Found = ITActiveSessions.Find(SessionID))
    {
        OutInfo = *Found;
        return true;
    }
    return false;
}

void UITDedicatedServerManager::ITCleanupFinishedSessions()
{
    TArray<FString> SessionsToRemove;
    const FDateTime CurrentTime = FDateTime::Now();

    for (const auto& SessionPair : ITActiveSessions)
    {
        const FITGameSessionInfo& SessionInfo = SessionPair.Value;
        const FTimespan ElapsedTime = CurrentTime - SessionInfo.CreatedTime;

        if (ElapsedTime.GetTotalHours() > 2.0)
        {
            SessionsToRemove.Add(SessionPair.Key);
        }
    }

    for (const FString& SessionID : SessionsToRemove)
    {
        if (FProcHandle* Handle = ITServerHandles.Find(SessionID))
        {
            // 필요 시 종료/정리 로직 추가 가능
            // FPlatformProcess::TerminateProc(*Handle, true);
            // FPlatformProcess::CloseProc(*Handle);
            ITServerHandles.Remove(SessionID);
        }

        ITActiveSessions.Remove(SessionID);
        UE_LOG(LogTemp, Log, TEXT("Cleaned up finished session: %s"), *SessionID);
    }
}

TArray<FITGameSessionInfo> UITDedicatedServerManager::ITGetActiveSessionsInfo()
{
    TArray<FITGameSessionInfo> ActiveSessions;
    ActiveSessions.Reserve(ITActiveSessions.Num());
    for (const auto& SessionPair : ITActiveSessions)
    {
        ActiveSessions.Add(SessionPair.Value);
    }
    return ActiveSessions;
}

bool UITDedicatedServerManager::ITSpawnDedicatedServer(const FString& SessionID, EITMatchmakingMode Mode)
{
    FITGameSessionInfo* FoundInfo = ITActiveSessions.Find(SessionID);
    if (!FoundInfo)
    {
        UE_LOG(LogTemp, Error, TEXT("ITSpawnDedicatedServer failed: Session not found %s"), *SessionID);
        return false;
    }

    const int32 Port = FoundInfo->ServerPort;
    const FString MapPath = TEXT("/Game/ImitationTrigger/Network/Test/Third/ThirdPerson/Maps/ThirdPersonMap");
    const FString ServerIP = TEXT("127.0.0.1");

#if WITH_EDITOR
    // 에디터(PIE) 로컬 테스트:
    // UnrealEditor.exe "<uproject>" /Game/.../ThirdPersonMap -server -log
    const FString EditorExe = FPlatformProcess::GenerateApplicationPath(TEXT("UnrealEditor"), EBuildConfiguration::Development);
    const FString ProjectArg = FString::Printf(TEXT("\"%s\""), *FPaths::ConvertRelativePathToFull(FPaths::GetProjectFilePath()));

    // 에디터 서버는 맵 인자 + -server -log가 안정적.
    // 포트를 강제로 바꾸려면 추가 인자(-Port 또는 -port)의 지원 여부가 빌드/버전에 따라 다를 수 있음.
    FString CmdLine = FString::Printf(TEXT("%s %s -server -log"), *ProjectArg, *MapPath);

    FProcHandle Handle = FPlatformProcess::CreateProc(
        *EditorExe, *CmdLine, true, false, false, nullptr, 0, *FPaths::ProjectDir(), nullptr
    );
#else
    // 패키지 서버 타겟 사용(파일명/경로 환경에 맞게 조정)
    // 예: Binaries/Win64/ProjectNameServer.exe /Game/.../ThirdPersonMap -log -port=7777
    const FString ServerExe = FPaths::Combine(FPaths::ProjectDir(), TEXT("Binaries/Win64/ProjectNameServer.exe"));
    const FString CmdLine = FString::Printf(TEXT("%s -log -port=%d"), *MapPath, Port);

    FProcHandle Handle = FPlatformProcess::CreateProc(
        *ServerExe, *CmdLine, true, false, false, nullptr, 0, nullptr, nullptr
    );
#endif

    if (!Handle.IsValid())
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to launch dedicated server process for session %s (port %d)"), *SessionID, Port);
        return false;
    }

    // 성공 시 주소 정보 보정
    FoundInfo->ServerIP = ServerIP;
    FoundInfo->ServerPort = Port;

    ITServerHandles.Add(SessionID, Handle);

    UE_LOG(LogTemp, Log, TEXT("Dedicated server spawned. Session=%s, %s:%d"), *SessionID, *ServerIP, Port);
    return true;
}

void UITDedicatedServerManager::ITNotifyPlayersOfGameStart(const TArray<FITPlayerMatchInfo>& Players, const FITGameSessionInfo& SessionInfo)
{
    const FString ServerAddress = FString::Printf(TEXT("%s:%d"), *SessionInfo.ServerIP, SessionInfo.ServerPort);
    for (const FString& PlayerID : SessionInfo.PlayerIDs)
    {
        UE_LOG(LogTemp, Log, TEXT("Notifying player %s to connect to %s"), *PlayerID, *ServerAddress);
    }
}

FString UITDedicatedServerManager::ITGenerateSessionID()
{
    const FGuid NewGuid = FGuid::NewGuid();
    return FString::Printf(TEXT("IT_%s"), *NewGuid.ToString(EGuidFormats::DigitsWithHyphensInBraces));
}

FString UITDedicatedServerManager::ITGetMapNameForMode(EITMatchmakingMode Mode)
{
    return TEXT("/Game/ImitationTrigger/Network/Test/Third/ThirdPerson/Maps/ThirdPersonMap");
}

int32 UITDedicatedServerManager::ITGetPortForSession()
{
    static int32 BasePort = 7777;
    return BasePort + (ITActiveSessions.Num() % 100);
}