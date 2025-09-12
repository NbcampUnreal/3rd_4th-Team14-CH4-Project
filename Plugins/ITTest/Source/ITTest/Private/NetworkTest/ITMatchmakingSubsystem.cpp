// Fill out your copyright notice in the Description page of Project Settings.


#include "NetworkTest/ITMatchmakingSubsystem.h"
#include "NetworkTest/ITDedicatedServerManager.h"
#include "NetworkTest/ITTestPlayerController.h"
#include "GameFramework/PlayerState.h"

void UITMatchmakingSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);

    ITSoloQueue.Empty();
    ITDuoQueue.Empty();
    ITTrioQueue.Empty();
    ITSquadQueue.Empty();

    if (UWorld* World = GetWorld())
    {
        World->GetTimerManager().SetTimer(
            ITMatchmakingTimerHandle,
            this,
            &UITMatchmakingSubsystem::ITProcessMatchmaking,
            2.0f,
            true
        );
    }

    // 테스트 빠른 진행
    ITMinimumPlayersForTimeout = 1;       // 1명만 있어도 타임아웃 경로에서 시작
    ITMatchmakingTimeoutSeconds = 5.0f;   // 5초 후 자동 시작

    UE_LOG(LogTemp, Warning, TEXT("IT Matchmaking Subsystem Initialized"));
}

void UITMatchmakingSubsystem::Deinitialize()
{
    if (UWorld* World = GetWorld())
    {
        World->GetTimerManager().ClearTimer(ITMatchmakingTimerHandle);
    }

    ITSoloQueue.Empty();
    ITDuoQueue.Empty();
    ITTrioQueue.Empty();
    ITSquadQueue.Empty();
    ITActiveRooms.Empty();

    Super::Deinitialize();
}

TArray<FITPlayerMatchInfo>* UITMatchmakingSubsystem::ITGetQueueForMode(EITMatchmakingMode Mode)
{
    switch (Mode)
    {
    case EITMatchmakingMode::Solo: return &ITSoloQueue;
    case EITMatchmakingMode::Duo: return &ITDuoQueue;
    case EITMatchmakingMode::Trio: return &ITTrioQueue;
    case EITMatchmakingMode::Squad: return &ITSquadQueue;
    default: return nullptr;
    }
}

const TArray<FITPlayerMatchInfo>* UITMatchmakingSubsystem::ITGetQueueForMode(EITMatchmakingMode Mode) const
{
    switch (Mode)
    {
    case EITMatchmakingMode::Solo: return &ITSoloQueue;
    case EITMatchmakingMode::Duo: return &ITDuoQueue;
    case EITMatchmakingMode::Trio: return &ITTrioQueue;
    case EITMatchmakingMode::Squad: return &ITSquadQueue;
    default: return nullptr;
    }
}

TArray<FITPlayerMatchInfo>* UITMatchmakingSubsystem::ITGetQueueForModePtr(EITMatchmakingMode Mode)
{
    switch (Mode)
    {
    case EITMatchmakingMode::Solo:  return &ITSoloQueue;
    case EITMatchmakingMode::Duo:   return &ITDuoQueue;
    case EITMatchmakingMode::Trio:  return &ITTrioQueue;
    case EITMatchmakingMode::Squad: return &ITSquadQueue;
    default:                        return nullptr;
    }
}

void UITMatchmakingSubsystem::ITServerRequestMatchmaking_Implementation(const FITPlayerMatchInfo& PlayerInfo)
{
    UE_LOG(LogTemp, Warning, TEXT("SERVER RPC: Received matchmaking request from %s"), *PlayerInfo.PlayerID);
    ITRequestMatchmaking(PlayerInfo);
}

void UITMatchmakingSubsystem::ITClientUpdateQueueInfo_Implementation(EITMatchmakingMode Mode, int32 QueueCount, int32 PlayerPosition)
{
    UE_LOG(LogTemp, Log, TEXT("CLIENT RPC: Queue update - Mode: %s, Count: %d, Position: %d"),
        *ITGetGameModeDisplayName(Mode), QueueCount, PlayerPosition);

    const FString StatusMessage = FString::Printf(TEXT("Waiting User: %d / %d"), PlayerPosition, QueueCount);
    OnITMatchmakingStatusChanged.Broadcast(StatusMessage);
}

void UITMatchmakingSubsystem::ITMulticastMatchmakingStatus_Implementation(const FString& Status)
{
    UE_LOG(LogTemp, Log, TEXT("MULTICAST RPC: Status update - %s"), *Status);
    OnITMatchmakingStatusChanged.Broadcast(Status);
}

bool UITMatchmakingSubsystem::ITRequestMatchmaking(const FITPlayerMatchInfo& PlayerInfo)
{
    if (GetWorld()->GetNetMode() == NM_DedicatedServer ||
        GetWorld()->GetNetMode() == NM_ListenServer)
    {
        if (ITIsInQueue(PlayerInfo.PlayerID))
        {
            return false;
        }

        if (TArray<FITPlayerMatchInfo>* Queue = ITGetQueueForMode(PlayerInfo.PreferredMode))
        {
            FITPlayerMatchInfo NewPlayerInfo = PlayerInfo;
            NewPlayerInfo.WaitTime = 0.0f;
            Queue->Add(NewPlayerInfo);

            ITBroadcastQueueUpdate(PlayerInfo.PreferredMode);
            ITLastPlayerJoinTime.Add(PlayerInfo.PreferredMode, FDateTime::UtcNow());

            UE_LOG(LogTemp, Warning, TEXT("SERVER: Player %s added to %s queue (Total: %d)"),
                *PlayerInfo.PlayerID, *ITGetGameModeDisplayName(PlayerInfo.PreferredMode), Queue->Num());

            return true;
        }
    }
    else
    {
        UE_LOG(LogTemp, Log, TEXT("CLIENT: Requesting matchmaking for %s"), *PlayerInfo.PlayerID);
        return true;
    }

    return false;
}

void UITMatchmakingSubsystem::ITCancelMatchmaking(const FString& PlayerID)
{
    bool bFoundAndRemoved = false;

    TArray<TArray<FITPlayerMatchInfo>*> AllQueues = {
        &ITSoloQueue, &ITDuoQueue, &ITTrioQueue, &ITSquadQueue
    };

    for (TArray<FITPlayerMatchInfo>* Queue : AllQueues)
    {
        for (int32 i = Queue->Num() - 1; i >= 0; i--)
        {
            if ((*Queue)[i].PlayerID == PlayerID)
            {
                Queue->RemoveAt(i);
                bFoundAndRemoved = true;
                break;
            }
        }
        if (bFoundAndRemoved) break;
    }

    if (bFoundAndRemoved)
    {
        OnITMatchmakingStatusChanged.Broadcast(TEXT("Cancelled matchmaking."));
        UE_LOG(LogTemp, Log, TEXT("Player %s removed from matchmaking queue"), *PlayerID);
    }
}

bool UITMatchmakingSubsystem::ITIsInQueue(const FString& PlayerID)
{
    TArray<const TArray<FITPlayerMatchInfo>*> AllQueues = {
        &ITSoloQueue, &ITDuoQueue, &ITTrioQueue, &ITSquadQueue
    };

    for (const TArray<FITPlayerMatchInfo>* Queue : AllQueues)
    {
        for (const FITPlayerMatchInfo& PlayerInfo : *Queue)
        {
            if (PlayerInfo.PlayerID == PlayerID)
            {
                return true;
            }
        }
    }
    return false;
}

int32 UITMatchmakingSubsystem::ITGetQueuePosition(const FString& PlayerID, EITMatchmakingMode Mode)
{
    const TArray<FITPlayerMatchInfo>* Queue = ITGetQueueForMode(Mode);
    if (!Queue) return -1;

    for (int32 i = 0; i < Queue->Num(); i++)
    {
        if ((*Queue)[i].PlayerID == PlayerID)
        {
            return i + 1;
        }
    }
    return -1;
}

int32 UITMatchmakingSubsystem::ITGetQueueCount(EITMatchmakingMode Mode)
{
    const TArray<FITPlayerMatchInfo>* Queue = ITGetQueueForMode(Mode);
    return Queue ? Queue->Num() : 0;
}

void UITMatchmakingSubsystem::ITBroadcastQueueUpdate(EITMatchmakingMode Mode)
{
    UWorld* World = GetWorld();
    if (!World) return;
    const ENetMode NM = World->GetNetMode();
    if (NM != NM_DedicatedServer && NM != NM_ListenServer) return;

    TArray<FITPlayerMatchInfo>* Queue = ITGetQueueForModePtr(Mode);
    if (!Queue) return;

    TMap<FString, int32> PositionByPlayer;
    for (int32 i = 0; i < Queue->Num(); ++i)
    {
        PositionByPlayer.Add((*Queue)[i].PlayerID, i + 1);
    }

    for (FConstPlayerControllerIterator It = World->GetPlayerControllerIterator(); It; ++It)
    {
        if (AITTestPlayerController* PC = Cast<AITTestPlayerController>(It->Get()))
        {
            FString TargetPlayerID;
            if (APlayerState* PS = PC->PlayerState)
            {
                TargetPlayerID = PS->GetPlayerName();
            }

            const int32* PosPtr = PositionByPlayer.Find(TargetPlayerID);
            const int32 Position = PosPtr ? *PosPtr : -1;

            PC->Client_ITUpdateQueueInfo(Mode, Queue->Num(), Position);
        }
    }
}

void UITMatchmakingSubsystem::ITProcessMatchmaking()
{
    if (GetWorld()->GetNetMode() != NM_DedicatedServer &&
        GetWorld()->GetNetMode() != NM_ListenServer)
    {
        return;
    }

    TArray<TArray<FITPlayerMatchInfo>*> AllQueues = {
        &ITSoloQueue, &ITDuoQueue, &ITTrioQueue, &ITSquadQueue
    };

    for (TArray<FITPlayerMatchInfo>* Queue : AllQueues)
    {
        for (FITPlayerMatchInfo& PlayerInfo : *Queue)
        {
            PlayerInfo.WaitTime += 2.0f;
        }
    }

    for (EITMatchmakingMode Mode : { EITMatchmakingMode::Solo, EITMatchmakingMode::Duo, EITMatchmakingMode::Trio, EITMatchmakingMode::Squad })
    {
        const TArray<FITPlayerMatchInfo>* Queue = ITGetQueueForMode(Mode);
        if (Queue && Queue->Num() > 0)
        {
            UE_LOG(LogTemp, Warning, TEXT("SERVER: Processing %s queue - %d players"),
                *ITGetGameModeDisplayName(Mode), Queue->Num());

            if (ITLastPlayerJoinTime.Contains(Mode))
            {
                float TimeSinceLastJoin = (FDateTime::UtcNow() - ITLastPlayerJoinTime[Mode]).GetTotalSeconds();
                UE_LOG(LogTemp, Warning, TEXT("Time since last join: %.1f seconds"), TimeSinceLastJoin);
            }
        }

        if (ITTryCreateMatch(Mode))
        {
            UE_LOG(LogTemp, Warning, TEXT("SERVER: Match created for mode: %s"),
                *ITGetGameModeDisplayName(Mode));
        }
    }
}

bool UITMatchmakingSubsystem::ITTryCreateMatch(EITMatchmakingMode Mode)
{
    TArray<FITPlayerMatchInfo>* Queue = ITGetQueueForMode(Mode);
    if (!Queue || Queue->Num() < ITMinimumPlayersForTimeout)
    {
        return false;
    }

    const int32 RequiredPlayers = ITGetRequiredPlayerCount(Mode);
    const FDateTime CurrentTime = FDateTime::UtcNow();

    bool bTimeoutReached = false;
    if (ITLastPlayerJoinTime.Contains(Mode))
    {
        FTimespan TimeSinceLastJoin = CurrentTime - ITLastPlayerJoinTime[Mode];
        bTimeoutReached = TimeSinceLastJoin.GetTotalSeconds() >= ITMatchmakingTimeoutSeconds;
    }

    const bool bShouldStartMatch = (Queue->Num() >= RequiredPlayers) ||
        (bTimeoutReached && Queue->Num() >= ITMinimumPlayersForTimeout);

    if (!bShouldStartMatch)
    {
        return false;
    }

    if (bTimeoutReached && Queue->Num() < RequiredPlayers)
    {
        UE_LOG(LogTemp, Warning, TEXT("Timeout reached! Starting match with %d players (Mode: %s)"),
            Queue->Num(), *ITGetGameModeDisplayName(Mode));
    }

    // 간단화를 위해 큐 전체 선택 (프로덕션에서는 팀로직/매칭품질 고려)
    Queue->Sort([](const FITPlayerMatchInfo& A, const FITPlayerMatchInfo& B) {
        return A.MMR < B.MMR;
        });

    TArray<FITPlayerMatchInfo> SelectedPlayers = *Queue;

    if (SelectedPlayers.Num() >= ITMinimumPlayersForTimeout)
    {
        ITCreateGameSession(SelectedPlayers, Mode);
        Queue->Empty();
        ITLastPlayerJoinTime.Remove(Mode);
        return true;
    }

    return false;
}

int32 UITMatchmakingSubsystem::ITGetRequiredPlayerCount(EITMatchmakingMode Mode)
{
    // 테스트: 솔로=1, 듀오=2, 트리오=3, 스쿼드=4
    switch (Mode)
    {
    case EITMatchmakingMode::Solo:  return 1;
    case EITMatchmakingMode::Duo:   return 2;
    case EITMatchmakingMode::Trio:  return 3;
    case EITMatchmakingMode::Squad: return 4;
    default: return 1;
    }
}

float UITMatchmakingSubsystem::ITCalculateMMRTolerance(float WaitTime)
{
    float BaseTolerance = 100.0f;
    float TimeMultiplier = FMath::Min(WaitTime / 30.0f, 5.0f);
    return BaseTolerance * (1.0f + TimeMultiplier);
}

bool UITMatchmakingSubsystem::ITCanPlayersMatch(const FITPlayerMatchInfo& Player1, const FITPlayerMatchInfo& Player2)
{
    float MMRTolerance = FMath::Max(
        ITCalculateMMRTolerance(Player1.WaitTime),
        ITCalculateMMRTolerance(Player2.WaitTime)
    );
    return FMath::Abs(Player1.MMR - Player2.MMR) <= MMRTolerance;
}

void UITMatchmakingSubsystem::ITCreateGameSession(const TArray<FITPlayerMatchInfo>& Players, EITMatchmakingMode Mode)
{
    if (UITDedicatedServerManager* ServerManager = GetGameInstance()->GetSubsystem<UITDedicatedServerManager>())
    {
        const FString SessionID = ServerManager->ITCreateGameSession(Players, Mode);
        if (!SessionID.IsEmpty())
        {
            FITGameSessionInfo Info;
            FString Address = TEXT("");
            if (ServerManager->ITGetSessionInfo(SessionID, Info))
            {
                Address = FString::Printf(TEXT("%s:%d"), *Info.ServerIP, Info.ServerPort);
            }

            for (const FITPlayerMatchInfo& Player : Players)
            {
                OnITMatchFound.Broadcast(SessionID, Address);
            }

            UE_LOG(LogTemp, Log, TEXT("Game session created: %s addr=%s with %d players"),
                *SessionID, *Address, Players.Num());
        }
    }
}

bool UITMatchmakingSubsystem::ITValidatePreformedTeam(const FITPlayerMatchInfo& TeamLeader, const TArray<FITPlayerMatchInfo>& Queue, const TArray<int32>& UsedIndices)
{
    int32 TeamSize = TeamLeader.TeamMembers.Num() + 1;
    int32 FoundMembers = 1;

    for (const FString& MemberID : TeamLeader.TeamMembers)
    {
        for (int32 i = 0; i < Queue.Num(); i++)
        {
            if (Queue[i].PlayerID == MemberID && !UsedIndices.Contains(i))
            {
                FoundMembers++;
                break;
            }
        }
    }

    return FoundMembers == TeamSize;
}

void UITMatchmakingSubsystem::ITAddPreformedTeam(const FITPlayerMatchInfo& TeamLeader, TArray<FITPlayerMatchInfo>& SelectedPlayers, const TArray<FITPlayerMatchInfo>& Queue, TArray<int32>& UsedIndices)
{
    SelectedPlayers.Add(TeamLeader);

    for (const FString& MemberID : TeamLeader.TeamMembers)
    {
        for (int32 i = 0; i < Queue.Num(); i++)
        {
            if (Queue[i].PlayerID == MemberID && !UsedIndices.Contains(i))
            {
                SelectedPlayers.Add(Queue[i]);
                UsedIndices.Add(i);
                break;
            }
        }
    }
}

FString UITMatchmakingSubsystem::ITGetGameModeDisplayName(EITMatchmakingMode Mode)
{
    switch (Mode)
    {
    case EITMatchmakingMode::Solo:  return TEXT("Solo");
    case EITMatchmakingMode::Duo:   return TEXT("Duo");
    case EITMatchmakingMode::Trio:  return TEXT("Trio");
    case EITMatchmakingMode::Squad: return TEXT("Squad");
    default: return TEXT("Unknown");
    }
}

void UITMatchmakingSubsystem::ITNotifyMatchFound(const TArray<FITPlayerMatchInfo>& Players, const FString& SessionID, const FString& ServerAddr)
{
    UWorld* World = GetWorld();
    if (!World) return;
    const ENetMode NM = World->GetNetMode();
    if (NM != NM_DedicatedServer && NM != NM_ListenServer) return;

    TSet<FString> PlayerSet;
    for (const auto& P : Players)
    {
        PlayerSet.Add(P.PlayerID);
    }

    for (FConstPlayerControllerIterator It = World->GetPlayerControllerIterator(); It; ++It)
    {
        if (AITTestPlayerController* PC = Cast<AITTestPlayerController>(It->Get()))
        {
            FString PCPlayerID;
            if (APlayerState* PS = PC->PlayerState)
            {
                PCPlayerID = PS->GetPlayerName();
            }
            if (PlayerSet.Contains(PCPlayerID))
            {
                PC->Client_ITMatchFound(SessionID, ServerAddr);
            }
        }
    }
}