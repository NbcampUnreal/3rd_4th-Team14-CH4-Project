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

protected:
	UPROPERTY()
	TObjectPtr<UUserWidget> TitleUIInstance;

	virtual void BeginPlay() override;

public:
	UFUNCTION(BlueprintCallable)
	void JoinServer(const FString& AddressOrMap);

	// 매칭 관련 RPC
	UFUNCTION(BlueprintCallable, Server, Reliable)
	void ServerRPC_StartMatchmaking();

	UFUNCTION(BlueprintCallable, Server, Reliable)
	void ServerRPC_JoinMatchmakingQueue();

	UFUNCTION(BlueprintCallable, Server, Reliable)
	void ServerRPC_LeaveMatchmakingQueue();


};
