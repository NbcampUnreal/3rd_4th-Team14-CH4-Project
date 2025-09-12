#include "NetworkTest/ITTestPlayerController.h"
#include "NetworkTest/ITMatchmakingSubsystem.h"
#include "Engine/Engine.h"
#include "Net/UnrealNetwork.h"

void AITTestPlayerController::Server_ITRequestMatchmaking_Implementation(const FITPlayerMatchInfo& PlayerInfo)
{
    ITPlayerID = PlayerInfo.PlayerID; // 서버에 식별자 기록(복제)
    if (UGameInstance* GI = GetGameInstance())
    {
        if (auto* MM = GI->GetSubsystem<UITMatchmakingSubsystem>())
        {
            MM->ITRequestMatchmaking(PlayerInfo); // 서버 컨텍스트에서 큐 추가
        }
    }
}

void AITTestPlayerController::Client_ITUpdateQueueInfo_Implementation(EITMatchmakingMode Mode, int32 QueueCount, int32 PlayerPosition)
{
    UE_LOG(LogTemp, Log, TEXT("CLIENT RPC: Queue update Mode=%d, Count=%d, Pos=%d"), (int32)Mode, QueueCount, PlayerPosition);
}

void AITTestPlayerController::Client_ITMatchFound_Implementation(const FString& SessionID, const FString& ServerAddress)
{
    UE_LOG(LogTemp, Log, TEXT("CLIENT RPC: Match found SessionID=%s Addr=%s"), *SessionID, *ServerAddress);
}

void AITTestPlayerController::Client_ITMatchmakingStatus_Implementation(const FString& Status)
{
    UE_LOG(LogTemp, Log, TEXT("CLIENT RPC: MM Status: %s"), *Status);
}

void AITTestPlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME(AITTestPlayerController, ITPlayerID);
}
