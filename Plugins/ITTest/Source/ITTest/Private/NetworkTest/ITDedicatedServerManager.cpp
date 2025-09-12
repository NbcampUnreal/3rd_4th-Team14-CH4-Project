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

    // ���� ���� �� Spawn (Spawn���� ���� ����)
    ITActiveSessions.Add(SessionID, SessionInfo);

    // ���� ���� ���� ���μ��� ����
    if (ITSpawnDedicatedServer(SessionID, Mode))
    {
        // �κ�/���ɽ�Ʈ������ ���忡�� ServerTravel�� ���� ȣ������ ����
        ITNotifyPlayersOfGameStart(Players, ITActiveSessions[SessionID]);

        UE_LOG(LogTemp, Warning, TEXT("Game session created successfully: %s with %d players"),
            *SessionID, Players.Num());
        return SessionID;
    }

    // ���� �� �ѹ�
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
            // �ʿ� �� ����/���� ���� �߰� ����
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
    // ������(PIE) ���� �׽�Ʈ:
    // UnrealEditor.exe "<uproject>" /Game/.../ThirdPersonMap -server -log
    const FString EditorExe = FPlatformProcess::GenerateApplicationPath(TEXT("UnrealEditor"), EBuildConfiguration::Development);
    const FString ProjectArg = FString::Printf(TEXT("\"%s\""), *FPaths::ConvertRelativePathToFull(FPaths::GetProjectFilePath()));

    // ������ ������ �� ���� + -server -log�� ������.
    // ��Ʈ�� ������ �ٲٷ��� �߰� ����(-Port �Ǵ� -port)�� ���� ���ΰ� ����/������ ���� �ٸ� �� ����.
    FString CmdLine = FString::Printf(TEXT("%s %s -server -log"), *ProjectArg, *MapPath);

    FProcHandle Handle = FPlatformProcess::CreateProc(
        *EditorExe, *CmdLine, true, false, false, nullptr, 0, *FPaths::ProjectDir(), nullptr
    );
#else
    // ��Ű�� ���� Ÿ�� ���(���ϸ�/��� ȯ�濡 �°� ����)
    // ��: Binaries/Win64/ProjectNameServer.exe /Game/.../ThirdPersonMap -log -port=7777
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

    // ���� �� �ּ� ���� ����
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