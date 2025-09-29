#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ITMatchTypes.h"
#include "ITLobbyGameMode.generated.h"

class APlayerController;

UCLASS()
class IMITATIONTRIGGER_API AITLobbyGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AITLobbyGameMode();

	UFUNCTION(BlueprintCallable)
	void StartMatchmaking();

	UFUNCTION(BlueprintCallable)
	void JoinMatchmakingQueue(APlayerController* PC);

	UFUNCTION(BlueprintCallable)
	void LeaveMatchmakingQueue(APlayerController* PC);

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	// 집 공인 IP 또는 도메인(명령행 -ExternalHost= 로 오버라이드)
	UPROPERTY(EditAnywhere, Category = "Dedicated")
	FString ExternalHost = TEXT("127.0.0.1"); // 나중에 내 IP로 변경 및 포트포워딩 하기

	// 로비 외 전용 서버 시작 포트(7778부터 증가)
	UPROPERTY(EditAnywhere, Category = "Dedicated")
	int32 BaseServerPort = 7778;

	// 매치용 맵 이름(기존 코드 흐름에 맞춤)
	UPROPERTY(EditAnywhere, Category = "Dedicated")
	FString MatchMap = TEXT("ThirdPersonMap");

	// 최소 시작 인원
	UPROPERTY(EditAnywhere, Category = "Matchmaking")
	int32 MinPlayersToStart = 2;

	// 에디터/로컬 테스트 시 서버 실행 파일 경로를 강제 지정(사용 시 이 경로 최우선)
	UPROPERTY(EditAnywhere, Category = "Dedicated")
	FString OverrideEditorServerExe = TEXT("E:/Unreal Projects/NBC/ImitationTrigger_Packaged2/WindowsServer/ImitationTriggerServer.exe");

	// 패키징된 기본 서버 실행 파일 경로(플랫폼별 기본값, 필요 시 에디터에서 교체 가능)
	UPROPERTY(EditAnywhere, Category = "Dedicated")
	FString DefaultPackagedServerExeWin64 = TEXT("Binaries/Win64/ImitationTriggerServer.exe");
	UPROPERTY(EditAnywhere, Category = "Dedicated")
	FString DefaultPackagedServerExeLinux = TEXT("Binaries/Linux/ImitationTriggerServer");

	// 대기열
	UPROPERTY()
	TArray<TWeakObjectPtr<APlayerController>> MatchmakingQueue;

	// 포트 할당(간단 증가형)
	int32 GetNextPort();

	// DS 실행: URL 쿼리 + -Port 로 기동, 접속 주소 반환
	bool LaunchDedicatedServer(const FString& SessionID, int32 ExpectedPlayers, int32& OutPort, FString& OutConnectURL);

	// 맵?SessionID=...&MatchPlayers=... 형태(포트 제외)
	FString BuildServerURLArgs(const FString& SessionID, int32 ExpectedPlayers) const;

	// 실행 파일 경로 결정(Override → 패키징 기본 경로 순)
	bool ResolveServerExecutable(FString& OutServerExe) const;

private:
	int32 NextPort = 0;
};
