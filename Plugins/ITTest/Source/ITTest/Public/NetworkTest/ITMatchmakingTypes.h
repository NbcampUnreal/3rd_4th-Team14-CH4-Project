// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/DataTable.h"
#include "ITMatchmakingTypes.generated.h"

UENUM(BlueprintType)
enum class EITMatchmakingMode : uint8
{
    Solo     UMETA(DisplayName = "�ַ�"),
    Duo      UMETA(DisplayName = "���"),
    Trio     UMETA(DisplayName = "Ʈ����"),
    Squad    UMETA(DisplayName = "������")
};

USTRUCT(BlueprintType)
struct ITTEST_API FITPlayerMatchInfo
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    FString PlayerID;

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    int32 MMR;

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    EITMatchmakingMode PreferredMode;

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    TArray<FString> TeamMembers;

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    float WaitTime;

    // �⺻ ������
    FITPlayerMatchInfo()
    {
        PlayerID = TEXT("");
        MMR = 1000;
        PreferredMode = EITMatchmakingMode::Solo;
        WaitTime = 0.0f;
    }
};

USTRUCT(BlueprintType)
struct ITTEST_API FITMatchRoom
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    FString RoomID;

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    EITMatchmakingMode GameMode;

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    TArray<FITPlayerMatchInfo> Players;

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    int32 RequiredPlayerCount;

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    FDateTime CreatedTime;

    // �⺻ ������
    FITMatchRoom()
    {
        RoomID = TEXT("");
        GameMode = EITMatchmakingMode::Solo;
        RequiredPlayerCount = 60;
        CreatedTime = FDateTime::Now();
    }
};

USTRUCT(BlueprintType)
struct ITTEST_API FITGameSessionInfo
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    FString SessionID;

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    FString ServerIP;

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    int32 ServerPort;

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    EITMatchmakingMode GameMode;

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    TArray<FString> PlayerIDs;

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    FDateTime CreatedTime;

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    bool bIsActive;

    // �⺻ ������
    FITGameSessionInfo()
    {
        SessionID = TEXT("");
        ServerIP = TEXT("127.0.0.1");
        ServerPort = 7777;
        GameMode = EITMatchmakingMode::Solo;
        CreatedTime = FDateTime::Now();
        bIsActive = true;
    }
};

UCLASS()
class ITTEST_API UITMatchmakingTypes : public UObject
{
	GENERATED_BODY()
	
};
