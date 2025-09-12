#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "NetworkTest/ITMatchmakingTypes.h"
#include "ITDedicatedServerManager.generated.h"

UCLASS()
class ITTEST_API UITDedicatedServerManager : public UGameInstanceSubsystem
{
    GENERATED_BODY()

public:
    virtual void Initialize(FSubsystemCollectionBase& Collection) override;

    UFUNCTION(BlueprintCallable, Category = "IT Server Management")
    FString ITCreateGameSession(const TArray<FITPlayerMatchInfo>& Players, EITMatchmakingMode Mode);

    UFUNCTION(BlueprintCallable, Category = "IT Server Management")
    bool ITGetSessionInfo(const FString& SessionID, FITGameSessionInfo& OutInfo);

    UFUNCTION(BlueprintCallable, Category = "IT Server Management")
    void ITCleanupFinishedSessions();

    UFUNCTION(BlueprintCallable, Category = "IT Server Management")
    TArray<FITGameSessionInfo> ITGetActiveSessionsInfo();

private:
    UPROPERTY()
    TMap<FString, FITGameSessionInfo> ITActiveSessions;

    bool ITSpawnDedicatedServer(const FString& SessionID, EITMatchmakingMode Mode);
    void ITNotifyPlayersOfGameStart(const TArray<FITPlayerMatchInfo>& Players, const FITGameSessionInfo& SessionInfo);
    FString ITGenerateSessionID();
    FString ITGetMapNameForMode(EITMatchmakingMode Mode);
    int32 ITGetPortForSession();

    // (선택) 서버 프로세스 핸들 관리: 세션ID -> ProcHandle
    TMap<FString, FProcHandle> ITServerHandles;
};