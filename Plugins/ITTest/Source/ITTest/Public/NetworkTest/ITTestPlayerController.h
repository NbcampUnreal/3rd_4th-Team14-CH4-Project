// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ITMatchmakingTypes.h"
#include "ITTestPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class ITTEST_API AITTestPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
    UFUNCTION(Server, Reliable)
    void Server_ITRequestMatchmaking(const FITPlayerMatchInfo& PlayerInfo);

    // 서버 → 클라: 큐 업데이트
    UFUNCTION(Client, Reliable)
    void Client_ITUpdateQueueInfo(EITMatchmakingMode Mode, int32 QueueCount, int32 PlayerPosition);

    // 서버 → 클라: 매치 성사 알림
    UFUNCTION(Client, Reliable)
    void Client_ITMatchFound(const FString& SessionID, const FString& ServerAddress);

    // 서버 → 클라: 상태 메시지
    UFUNCTION(Client, Reliable)
    void Client_ITMatchmakingStatus(const FString& Status);

    // 서버에서 식별용으로 보관(복제)
    UPROPERTY(Replicated)
    FString ITPlayerID;

protected:
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
