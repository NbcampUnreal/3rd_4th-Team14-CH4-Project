// Fill out your copyright notice in the Description page of Project Settings.


#include "NetworkTest/ITGameInstance.h"
#include "NetworkTest/ITMatchmakingSubsystem.h"
#include "Engine/Engine.h"
#include "NetworkTest/ITMatchmakingWidget.h"

UITGameInstance::UITGameInstance()
{
    bITIsCurrentlyMatchmaking = false;
    ITCurrentMatchMode = EITMatchmakingMode::Solo;
}

void UITGameInstance::Init()
{
    Super::Init();
    /*
    ITCurrentPlayerInfo.PlayerID = TEXT("");
    ITCurrentPlayerInfo.MMR = 1000;
    ITCurrentPlayerInfo.PreferredMode = EITMatchmakingMode::Solo;
    ITCurrentPlayerInfo.WaitTime = 0.0f;

    if (UITMatchmakingSubsystem* MatchmakingSystem = GetSubsystem<UITMatchmakingSubsystem>())
    {
        MatchmakingSystem->OnITMatchFound.AddDynamic(this, &UITGameInstance::ITOnMatchFound);
        MatchmakingSystem->OnITMatchmakingStatusChanged.AddDynamic(this, &UITGameInstance::ITOnMatchmakingStatusChanged);
    }
    */
    FString UniquePlayerID = FString::Printf(TEXT("Player_%s_%d"),
        *FPlatformMisc::GetLoginId(),
        FMath::RandRange(1000, 9999));

    ITInitializePlayerInfo(UniquePlayerID, 1000);

    if (UITMatchmakingSubsystem* MatchmakingSystem = GetSubsystem<UITMatchmakingSubsystem>())
    {
        MatchmakingSystem->OnITMatchFound.AddDynamic(this, &UITGameInstance::ITOnMatchFound);
        MatchmakingSystem->OnITMatchmakingStatusChanged.AddDynamic(this, &UITGameInstance::ITOnMatchmakingStatusChanged);
    }

    UE_LOG(LogTemp, Warning, TEXT("IT Game Instance Initialized with Player: %s"), *UniquePlayerID);
}

void UITGameInstance::Shutdown()
{
    if (bITIsCurrentlyMatchmaking)
    {
        ITStopMatchmaking();
    }

    Super::Shutdown();
}

void UITGameInstance::ITStartMatchmaking(EITMatchmakingMode Mode)
{
    if (bITIsCurrentlyMatchmaking)
    {
        UE_LOG(LogTemp, Warning, TEXT("Already in matchmaking"));
        return;
    }

    if (ITCurrentPlayerInfo.PlayerID.IsEmpty())
    {
        UE_LOG(LogTemp, Error, TEXT("Player ID is not set"));
        return;
    }

    ITCurrentPlayerInfo.PreferredMode = Mode;
    ITCurrentPlayerInfo.WaitTime = 0.0f;
    ITCurrentMatchMode = Mode;

    if (UITMatchmakingSubsystem* MatchmakingSystem = GetSubsystem<UITMatchmakingSubsystem>())
    {
        // 클라이언트는 서버 RPC 호출
        if (GetWorld()->GetNetMode() == NM_Client)
        {
            MatchmakingSystem->ITServerRequestMatchmaking(ITCurrentPlayerInfo);
            bITIsCurrentlyMatchmaking = true;
            UE_LOG(LogTemp, Warning, TEXT("CLIENT: Sent RPC matchmaking request"));
        }
        else
        {
            // 서버/Listen Server는 직접 처리
            if (MatchmakingSystem->ITRequestMatchmaking(ITCurrentPlayerInfo))
            {
                bITIsCurrentlyMatchmaking = true;
                UE_LOG(LogTemp, Warning, TEXT("SERVER: Direct matchmaking started"));
            }
            else
            {
                UE_LOG(LogTemp, Error, TEXT("Failed to start matchmaking"));
            }
        }
    }
}

void UITGameInstance::ITStopMatchmaking()
{
    if (!bITIsCurrentlyMatchmaking)
    {
        return;
    }

    if (UITMatchmakingSubsystem* MatchmakingSystem = GetSubsystem<UITMatchmakingSubsystem>())
    {
        MatchmakingSystem->ITCancelMatchmaking(ITCurrentPlayerInfo.PlayerID);
    }

    bITIsCurrentlyMatchmaking = false;
    UE_LOG(LogTemp, Log, TEXT("Stopped matchmaking"));
}

bool UITGameInstance::ITIsCurrentlyMatchmaking()
{
	return bITIsCurrentlyMatchmaking;
}

void UITGameInstance::ITInitializePlayerInfo(const FString& PlayerID, int32 InitialMMR)
{
    ITCurrentPlayerInfo.PlayerID = PlayerID;
    ITCurrentPlayerInfo.MMR = InitialMMR;
    ITCurrentPlayerInfo.PreferredMode = EITMatchmakingMode::Solo;
    ITCurrentPlayerInfo.TeamMembers.Empty();
    ITCurrentPlayerInfo.WaitTime = 0.0f;

    UE_LOG(LogTemp, Log, TEXT("Player info initialized: %s (MMR: %d)"), *PlayerID, InitialMMR);
}


void UITGameInstance::ITOnMatchFound(FString SessionID, FString ServerAddress)
{
    bITIsCurrentlyMatchmaking = false;

    UE_LOG(LogTemp, Log, TEXT("Match found! Session: %s, Server: %s"), *SessionID, *ServerAddress);

    // 블루프린트에서 구현될 ITConnectToGameServer 호출
    ITConnectToGameServer(ServerAddress, SessionID);
}

void UITGameInstance::ITOnMatchmakingStatusChanged(FString Status)
{
    UE_LOG(LogTemp, Log, TEXT("Matchmaking status: %s"), *Status);
}
