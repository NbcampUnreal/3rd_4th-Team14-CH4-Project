#pragma once

#include "Player/ITPlayerController.h"
#include "ITBattlePlayerController.generated.h"

class UHUDWidget;


UCLASS()
class IMITATIONTRIGGER_API AITBattlePlayerController : public AITPlayerController
{
	GENERATED_BODY()
	
public:
	AITBattlePlayerController();
	virtual void BeginPlay() override;
	virtual void EndPlay(EEndPlayReason::Type EndPlayReason) override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Meta = (AllowPrivateAccess = true))
	TSubclassOf<UHUDWidget> HUDWidgetClass;

	UPROPERTY()
	TObjectPtr<UHUDWidget> HUDWidget;
};
