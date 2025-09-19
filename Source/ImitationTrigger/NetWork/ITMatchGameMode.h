#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Network/ITMatchTypes.h"
#include "ITMatchGameMode.generated.h"


UCLASS()
class IMITATIONTRIGGER_API AITMatchGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	AITMatchGameMode();

	virtual void PostLogin(APlayerController* NewPlayer) override;
	virtual void BeginPlay() override;

protected:
	UPROPERTY()
	FString CurrentSessionID;

	UPROPERTY()
	int32 ExpectedPlayerCount;

	UPROPERTY()
	TArray<APlayerController*> MatchPlayers;

	void ExtractSessionInfoFromURL();
	void StartMatchWhenReady();
	
	
};
