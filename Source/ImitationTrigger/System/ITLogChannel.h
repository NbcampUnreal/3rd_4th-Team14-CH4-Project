// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#pragma region NetLogging

IMITATIONTRIGGER_API DECLARE_LOG_CATEGORY_EXTERN(LogITNet, Log, All);

// ���� ���(Server/Client/Standalone)
//#define NETMODE_TCHAR ((GetNetMode() == ENetMode::NM_Client) ? *FString::Printf(TEXT("Client%02d"), UE::GetPlayInEditorID()) : ((GetNetMode() == ENetMode::NM_Standalone) ? TEXT("StandAlone") : TEXT("Server")))
//#define FUNCTION_TCHAR (ANSI_TO_TCHAR(__FUNCTION__))
//#define IT_LOG_NET(LogCategory, Verbosity, Format, ...) UE_LOG(LogCategory, Verbosity, TEXT("[%s] %s %s"), NETMODE_TCHAR, FUNCTION_TCHAR, *FString::Printf(Format, ##__VA_ARGS__))
//
//#define LOCAL_ROLE_TCHAR *(UEnum::GetValueAsString(TEXT("Engine.ENetRole"), GetLocalRole())) // Actor�� Role ���� (������ �ο�?)
//#define REMOTE_ROLE_TCHAR *(UEnum::GetValueAsString(TEXT("Engine.ENetRole"), GetRemoteRole()))
//#define IT_LOG_ROLE(LogCat, Verbosity, Format, ...) UE_LOG(LogCat, Verbosity, TEXT("[%s][%s/%s] %s %s"), NETMODE_TCHAR, LOCAL_ROLE_TCHAR, REMOTE_ROLE_TCHAR, FUNCTION_TCHAR, *FString::Printf(Format, ##__VA_ARGS__))


// ���� ���(Server/Client/Standalone) : NetMode �Ǻ�.
#define NETMODE_TCHAR \
    ((GetNetMode() == ENetMode::NM_Client) ? *FString::Printf(TEXT("Client%02d"), UE::GetPlayInEditorID()) : \
    ((GetNetMode() == ENetMode::NM_Standalone) ? TEXT("StandAlone") : TEXT("Server")))
// �Լ� �̸� ���.
#define FUNCTION_TCHAR (ANSI_TO_TCHAR(__FUNCTION__))

// �⺻ ��Ʈ��ũ �α� : ���� ��� + �Լ� �̸�
#define IT_LOG_NET(LogCategory, Verbosity, Format, ...) \
    UE_LOG(LogCategory, Verbosity, TEXT("[%s][%s] ") Format, \
    NETMODE_TCHAR, FUNCTION_TCHAR, ##__VA_ARGS__)

// Role �α� : Local/Remote Role ���� ����
#define LOCAL_ROLE_TCHAR *StaticEnum<ENetRole>()->GetValueAsString(GetLocalRole())
#define REMOTE_ROLE_TCHAR *StaticEnum<ENetRole>()->GetValueAsString(GetRemoteRole())

#define IT_LOG_ROLE(LogCat, Verbosity, Format, ...) \
    UE_LOG(LogCat, Verbosity, TEXT("[%s][%s/%s][%s] ") Format, \
    NETMODE_TCHAR, LOCAL_ROLE_TCHAR, REMOTE_ROLE_TCHAR, FUNCTION_TCHAR, ##__VA_ARGS__)


// Proxy �α� : AutonomousProxy / SimulatedProxy �� Proxy Ÿ�� Ȯ��
#define PROXY_TCHAR \
    ((GetLocalRole() == ROLE_AutonomousProxy) ? TEXT("AutonomousProxy") : \
    ((GetLocalRole() == ROLE_SimulatedProxy) ? TEXT("SimulatedProxy") : \
    *StaticEnum<ENetRole>()->GetValueAsString(GetLocalRole())))

#define IT_LOG_PROXY(LogCat, Verbosity, Format, ...) \
    UE_LOG(LogCat, Verbosity, TEXT("[%s][%s][%s] ") Format, \
    NETMODE_TCHAR, PROXY_TCHAR, FUNCTION_TCHAR, ##__VA_ARGS__)

// Connection �α� : ����/Ŭ���̾�Ʈ ���� ��ü �̸� ��� (Server���� ClientConnection��ü ����, Client���� ServerConnection��ü ����)
#define IT_LOG_CONN(LogCat, Verbosity, Conn, Format, ...) \
    if (Conn) { \
        UE_LOG(LogCat, Verbosity, TEXT("[%s][Conn:%s][%s] ") Format, \
        NETMODE_TCHAR, *Conn->GetName(), FUNCTION_TCHAR, ##__VA_ARGS__); \
    }

// RPC �α� : RPC�Լ� ȣ�� ����
#define IT_LOG_RPC(LogCat, Verbosity, RpcType, Format, ...) \
    UE_LOG(LogCat, Verbosity, TEXT("[%s][%sRPC][%s] ") Format, \
        NETMODE_TCHAR, TEXT(#RpcType), FUNCTION_TCHAR, ##__VA_ARGS__)














#pragma endregion