#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "NetworkTest/ITMatchmakingTypes.h"
#include "ITGameInstance.generated.h"

class UITMatchmakingSubsystem;

UCLASS()
class ITTEST_API UITGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
    UITGameInstance();

    virtual void Init() override;
    virtual void Shutdown() override;

    UPROPERTY(BlueprintReadWrite, Category = "IT Player Info")
    FITPlayerMatchInfo ITCurrentPlayerInfo;

    UPROPERTY(BlueprintReadWrite, Category = "IT Matchmaking State")
    bool bITIsCurrentlyMatchmaking;

    UPROPERTY(BlueprintReadWrite, Category = "IT Matchmaking State")
    EITMatchmakingMode ITCurrentMatchMode;

    UFUNCTION(BlueprintCallable, Category = "IT Matchmaking")
    void ITStartMatchmaking(EITMatchmakingMode Mode);

    UFUNCTION(BlueprintCallable, Category = "IT Matchmaking")
    void ITStopMatchmaking();

    UFUNCTION(BlueprintCallable, Category = "IT Matchmaking")
    bool ITIsCurrentlyMatchmaking();

    UFUNCTION(BlueprintCallable, Category = "IT Matchmaking")
    void ITInitializePlayerInfo(const FString& PlayerID, int32 InitialMMR = 1000);

    UFUNCTION(BlueprintImplementableEvent, Category = "IT Game Flow")
    void ITConnectToGameServer(const FString& ServerAddress, const FString& SessionID);

protected:
    UFUNCTION()
    void ITOnMatchFound(FString SessionID, FString ServerAddress);

    UFUNCTION()
    void ITOnMatchmakingStatusChanged(FString Status);


};
