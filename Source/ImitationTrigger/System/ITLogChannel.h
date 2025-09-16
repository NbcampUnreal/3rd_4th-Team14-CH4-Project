// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#pragma region NetLogging

IMITATIONTRIGGER_API DECLARE_LOG_CATEGORY_EXTERN(LogITNet, Log, All);

// 실행 모드(Server/Client/Standalone)
//#define NETMODE_TCHAR ((GetNetMode() == ENetMode::NM_Client) ? *FString::Printf(TEXT("Client%02d"), UE::GetPlayInEditorID()) : ((GetNetMode() == ENetMode::NM_Standalone) ? TEXT("StandAlone") : TEXT("Server")))
//#define FUNCTION_TCHAR (ANSI_TO_TCHAR(__FUNCTION__))
//#define IT_LOG_NET(LogCategory, Verbosity, Format, ...) UE_LOG(LogCategory, Verbosity, TEXT("[%s] %s %s"), NETMODE_TCHAR, FUNCTION_TCHAR, *FString::Printf(Format, ##__VA_ARGS__))
//
//#define LOCAL_ROLE_TCHAR *(UEnum::GetValueAsString(TEXT("Engine.ENetRole"), GetLocalRole())) // Actor의 Role 상태 (권한을 부여?)
//#define REMOTE_ROLE_TCHAR *(UEnum::GetValueAsString(TEXT("Engine.ENetRole"), GetRemoteRole()))
//#define IT_LOG_ROLE(LogCat, Verbosity, Format, ...) UE_LOG(LogCat, Verbosity, TEXT("[%s][%s/%s] %s %s"), NETMODE_TCHAR, LOCAL_ROLE_TCHAR, REMOTE_ROLE_TCHAR, FUNCTION_TCHAR, *FString::Printf(Format, ##__VA_ARGS__))


// 실행 모드(Server/Client/Standalone) : NetMode 판별.
#define NETMODE_TCHAR \
    ((GetNetMode() == ENetMode::NM_Client) ? *FString::Printf(TEXT("Client%02d"), UE::GetPlayInEditorID()) : \
    ((GetNetMode() == ENetMode::NM_Standalone) ? TEXT("StandAlone") : TEXT("Server")))
// 함수 이름 출력.
#define FUNCTION_TCHAR (ANSI_TO_TCHAR(__FUNCTION__))

// 기본 네트워크 로그 : 실행 모드 + 함수 이름
#define IT_LOG_NET(LogCategory, Verbosity, Format, ...) \
    UE_LOG(LogCategory, Verbosity, TEXT("[%s][%s] ") Format, \
    NETMODE_TCHAR, FUNCTION_TCHAR, ##__VA_ARGS__)

// Role 로그 : Local/Remote Role 상태 추적
#define LOCAL_ROLE_TCHAR *StaticEnum<ENetRole>()->GetValueAsString(GetLocalRole())
#define REMOTE_ROLE_TCHAR *StaticEnum<ENetRole>()->GetValueAsString(GetRemoteRole())

#define IT_LOG_ROLE(LogCat, Verbosity, Format, ...) \
    UE_LOG(LogCat, Verbosity, TEXT("[%s][%s/%s][%s] ") Format, \
    NETMODE_TCHAR, LOCAL_ROLE_TCHAR, REMOTE_ROLE_TCHAR, FUNCTION_TCHAR, ##__VA_ARGS__)


// Proxy 로그 : AutonomousProxy / SimulatedProxy 등 Proxy 타입 확인
#define PROXY_TCHAR \
    ((GetLocalRole() == ROLE_AutonomousProxy) ? TEXT("AutonomousProxy") : \
    ((GetLocalRole() == ROLE_SimulatedProxy) ? TEXT("SimulatedProxy") : \
    *StaticEnum<ENetRole>()->GetValueAsString(GetLocalRole())))

#define IT_LOG_PROXY(LogCat, Verbosity, Format, ...) \
    UE_LOG(LogCat, Verbosity, TEXT("[%s][%s][%s] ") Format, \
    NETMODE_TCHAR, PROXY_TCHAR, FUNCTION_TCHAR, ##__VA_ARGS__)

// Connection 로그 : 서버/클라이언트 연결 객체 이름 출력 (Server에는 ClientConnection객체 존재, Client에는 ServerConnection객체 존재)
#define IT_LOG_CONN(LogCat, Verbosity, Conn, Format, ...) \
    if (Conn) { \
        UE_LOG(LogCat, Verbosity, TEXT("[%s][Conn:%s][%s] ") Format, \
        NETMODE_TCHAR, *Conn->GetName(), FUNCTION_TCHAR, ##__VA_ARGS__); \
    }

// RPC 로그 : RPC함수 호출 추적
#define IT_LOG_RPC(LogCat, Verbosity, RpcType, Format, ...) \
    UE_LOG(LogCat, Verbosity, TEXT("[%s][%sRPC][%s] ") Format, \
        NETMODE_TCHAR, TEXT(#RpcType), FUNCTION_TCHAR, ##__VA_ARGS__)














#pragma endregion