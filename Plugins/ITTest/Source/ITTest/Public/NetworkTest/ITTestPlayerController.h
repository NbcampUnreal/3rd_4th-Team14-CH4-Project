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

    // ���� �� Ŭ��: ť ������Ʈ
    UFUNCTION(Client, Reliable)
    void Client_ITUpdateQueueInfo(EITMatchmakingMode Mode, int32 QueueCount, int32 PlayerPosition);

    // ���� �� Ŭ��: ��ġ ���� �˸�
    UFUNCTION(Client, Reliable)
    void Client_ITMatchFound(const FString& SessionID, const FString& ServerAddress);

    // ���� �� Ŭ��: ���� �޽���
    UFUNCTION(Client, Reliable)
    void Client_ITMatchmakingStatus(const FString& Status);

    // �������� �ĺ������� ����(����)
    UPROPERTY(Replicated)
    FString ITPlayerID;

protected:
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
