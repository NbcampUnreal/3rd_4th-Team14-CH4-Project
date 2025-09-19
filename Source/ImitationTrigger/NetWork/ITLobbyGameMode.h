#pragma once
#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Network/ITMatchTypes.h"
#include "HAL/CriticalSection.h"
#include "ITLobbyGameMode.generated.h"

class AITLobbyGameState;

USTRUCT(BlueprintType)
struct FLaunchedMatchProcess
{
	GENERATED_BODY()
	UPROPERTY() FString SessionID;
	UPROPERTY() int32 Port = 0;
	UPROPERTY() float LaunchTime = 0.f;
	UPROPERTY() bool bActive = false;
	FProcHandle ProcHandle;
	void* ReadPipe = nullptr;
	void* WritePipe = nullptr;

	FLaunchedMatchProcess() {}
	FLaunchedMatchProcess(const FString& InID, int32 InPort, float InLaunchTime, const FProcHandle& InHandle, void* InRead, void* InWrite)
		: SessionID(InID), Port(InPort), LaunchTime(InLaunchTime), bActive(true), ProcHandle(InHandle), ReadPipe(InRead), WritePipe(InWrite) {
	}
};

UCLASS()
class IMITATIONTRIGGER_API AITLobbyGameMode : public AGameModeBase
{
	GENERATED_BODY()
public:
	AITLobbyGameMode();
	virtual void PostLogin(APlayerController* NewPlayer) override;
	virtual void Logout(AController* Existing) override;
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UFUNCTION(BlueprintCallable)
	void StartMatchmaking();
	UFUNCTION(BlueprintCallable)
	void JoinMatchmakingQueue(APlayerController* Player);
	UFUNCTION(BlueprintCallable)
	void LeaveMatchmakingQueue(APlayerController* Player);

protected:
	UPROPERTY()
	TArray<TWeakObjectPtr<APlayerController>> AliveControllers;

	UPROPERTY()
	TArray<FMatchmakingPlayerInfo> MatchmakingQueue;

	UPROPERTY()
	TArray<FLaunchedMatchProcess> ActiveMatchProcesses;

	// 세션별 접속 URL 캐시 (127.0.0.1:Port)
	UPROPERTY()
	TMap<FString, FString> SessionIdToConnectURL;

	// 포트 카운터 (순차 할당용)
	UPROPERTY()
	int32 NextAvailablePort = 7887;

	// 포트 할당 동시성 보호
	mutable FCriticalSection PortAllocationMutex;

	FTimerHandle MainTimerHandle;
	FTimerHandle SessionCleanupTimerHandle;

	UPROPERTY(EditAnywhere, Category = "Dedicated")
	bool bForceDedicatedInEditor = true;

	UPROPERTY(EditAnywhere, Category = "Dedicated")
	FString OverrideEditorServerExe = TEXT("E:/Unreal Projects/NBC/ImitationTrigger_Packaged2/WindowsServer/ImitationTriggerServer.exe");

	UPROPERTY(EditAnywhere, Category = "Dedicated")
	FName MatchMapName = FName(TEXT("ThirdPersonMap"));

	UPROPERTY(EditAnywhere, Category = "Dedicated")
	int32 BaseServerPort = 7887;

	UPROPERTY(EditAnywhere, Category = "Dedicated")
	float ServerBootTimeout = 30.f;

	void OnMainTick();
	void OnSessionCleanupTick();
	void UpdateAliveCount();

	void StartGame(const TArray<FMatchmakingPlayerInfo>& PlayersToMove, const FString& NewSessionID);

	bool ShouldUseDedicatedPath() const;

	bool LaunchDedicatedServerProcess(const FString& SessionID, int32 ExpectedPlayers, int32& OutPort, FString& OutConnectURL, FLaunchedMatchProcess& OutProcess);

	// 포트 할당 (카운터 방식)
	bool GetAvailablePort(int32& OutPort);
	bool CanConnectToPort(int32 Port, const FString& Host = TEXT("127.0.0.1")) const;

	FString BuildServerURL(const FString& MapName, const FString& SessionID, int32 ExpectedPlayers, int32 Port) const;

	void ShutdownProcess(FLaunchedMatchProcess& Proc);
	void ShutdownProcessBySession(const FString& SessionID);
	void ShutdownAllProcesses();

	AITLobbyGameState* GetLobbyGS() const;
};
