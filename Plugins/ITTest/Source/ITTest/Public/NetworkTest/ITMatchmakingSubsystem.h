#pragma once
#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Engine/TimerHandle.h"
#include "NetworkTest/ITMatchmakingTypes.h"
#include "ITMatchmakingSubsystem.generated.h"

class UITDedicatedServerManager;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FITOnMatchFound, FString, SessionID, FString, ServerAddress);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FITOnMatchmakingStatusChanged, FString, Status);

UCLASS()
class ITTEST_API UITMatchmakingSubsystem : public UGameInstanceSubsystem
{
    GENERATED_BODY()
public:
    virtual void Initialize(FSubsystemCollectionBase& Collection) override;
    virtual void Deinitialize() override;

    UFUNCTION(BlueprintCallable, Category = "IT Matchmaking")
    bool ITRequestMatchmaking(const FITPlayerMatchInfo& PlayerInfo);

    UFUNCTION(BlueprintCallable, Category = "IT Matchmaking")
    void ITCancelMatchmaking(const FString& PlayerID);

    UFUNCTION(BlueprintCallable, Category = "IT Matchmaking")
    bool ITIsInQueue(const FString& PlayerID);

    UFUNCTION(BlueprintCallable, Category = "IT Matchmaking")
    int32 ITGetQueuePosition(const FString& PlayerID, EITMatchmakingMode Mode);

    UFUNCTION(BlueprintCallable, Category = "IT Matchmaking")
    int32 ITGetQueueCount(EITMatchmakingMode Mode);

    UFUNCTION(Server, Reliable)
    void ITServerRequestMatchmaking(const FITPlayerMatchInfo& PlayerInfo);

    UFUNCTION(Client, Reliable)
    void ITClientUpdateQueueInfo(EITMatchmakingMode Mode, int32 QueueCount, int32 PlayerPosition);

    UFUNCTION(NetMulticast, Reliable)
    void ITMulticastMatchmakingStatus(const FString& Status);

    UPROPERTY(BlueprintAssignable)
    FITOnMatchFound OnITMatchFound;

    UPROPERTY(BlueprintAssignable)
    FITOnMatchmakingStatusChanged OnITMatchmakingStatusChanged;

private:
    UPROPERTY() TArray<FITPlayerMatchInfo> ITSoloQueue;
    UPROPERTY() TArray<FITPlayerMatchInfo> ITDuoQueue;
    UPROPERTY() TArray<FITPlayerMatchInfo> ITTrioQueue;
    UPROPERTY() TArray<FITPlayerMatchInfo> ITSquadQueue;
    UPROPERTY() TArray<FITMatchRoom>       ITActiveRooms;

    FTimerHandle ITMatchmakingTimerHandle;

    UPROPERTY()
    TMap<EITMatchmakingMode, FDateTime> ITLastPlayerJoinTime;

    int32 ITMinimumPlayersForTimeout = 2;
    int32 ITRuquiredPlayersForMatch = 2;
    float ITMatchmakingTimeoutSeconds = 10.0f;

    UFUNCTION() void ITProcessMatchmaking();

    TArray<FITPlayerMatchInfo>* ITGetQueueForMode(EITMatchmakingMode Mode);
    const TArray<FITPlayerMatchInfo>* ITGetQueueForMode(EITMatchmakingMode Mode) const;

    bool  ITTryCreateMatch(EITMatchmakingMode Mode);
    int32 ITGetRequiredPlayerCount(EITMatchmakingMode Mode);
    float ITCalculateMMRTolerance(float WaitTime);
    bool  ITCanPlayersMatch(const FITPlayerMatchInfo& Player1, const FITPlayerMatchInfo& Player2);
    void  ITCreateGameSession(const TArray<FITPlayerMatchInfo>& Players, EITMatchmakingMode Mode);
    bool  ITValidatePreformedTeam(const FITPlayerMatchInfo& TeamLeader, const TArray<FITPlayerMatchInfo>& Queue, const TArray<int32>& UsedIndices);
    void  ITAddPreformedTeam(const FITPlayerMatchInfo& TeamLeader, TArray<FITPlayerMatchInfo>& SelectedPlayers, const TArray<FITPlayerMatchInfo>& Queue, TArray<int32>& UsedIndices);
    FString ITGetGameModeDisplayName(EITMatchmakingMode Mode);

public:
    // 요청하신 보조 API
    TArray<FITPlayerMatchInfo>* ITGetQueueForModePtr(EITMatchmakingMode Mode);
    void ITBroadcastQueueUpdate(EITMatchmakingMode Mode);
    void ITNotifyMatchFound(const TArray<FITPlayerMatchInfo>& Players, const FString& SessionID, const FString& ServerAddr);
};
