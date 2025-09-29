#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "Network/ITMatchTypes.h"
#include "ITBattleGameMode.generated.h"


UCLASS()
class IMITATIONTRIGGER_API AITBattleGameMode : public AGameMode
{
	GENERATED_BODY()
	
	AITBattleGameMode();

	virtual void BeginPlay() override;
	virtual void PostLogin(APlayerController* NewPlayer) override;

protected:
	// 로비에서 URL로 넘겨주는 값
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Match")
	FString CurrentSessionID;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Match")
	int32 ExpectedPlayerCount = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Match")
	TArray<TObjectPtr<APlayerController>> MatchPlayers;

	void ExtractSessionInfoFromURL(); // ThirdPersonMap?SessionID=...&MatchPlayers=...
	void StartMatchWhenReady();

};
