#pragma once
#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ITTitlePlayerController.generated.h"

class UUserWidget;

UCLASS()
class IMITATIONTRIGGER_API AITTitlePlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> TitleUIClass;

	UFUNCTION(BlueprintCallable)
	bool IsInMatchmakingQueue() const { return bIsInMatchmakingQueue; }

protected:
	UPROPERTY()
	TObjectPtr<UUserWidget> TitleUIInstance;

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	// UI 설정
	void SetupTitleUI();

	// 매칭 상태
	UPROPERTY()
	bool bIsInMatchmakingQueue = false;

public:
	UFUNCTION(BlueprintCallable)
	void JoinServer(const FString& AddressOrMap);

	// 입력 모드 관리
	UFUNCTION(BlueprintCallable)
	void SetGameInputMode();

	// 매칭 관련 RPC
	UFUNCTION(BlueprintCallable, Server, Reliable)
	void ServerRPC_StartMatchmaking();

	UFUNCTION(BlueprintCallable, Server, Reliable)
	void ServerRPC_JoinMatchmakingQueue();

	UFUNCTION(BlueprintCallable, Server, Reliable)
	void ServerRPC_LeaveMatchmakingQueue();

	// 클라이언트 RPC (서버에서 클라이언트로 매칭 상태 통지)
	UFUNCTION(Client, Reliable)
	void ClientRPC_UpdateMatchmakingState(bool bIsInQueue);


};
