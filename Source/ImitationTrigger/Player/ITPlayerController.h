#pragma once

#include "GameFramework/PlayerController.h"
#include "ITPlayerController.generated.h"

class UITAbilitySystemComponent;
class AITPlayerState;

UCLASS()
class IMITATIONTRIGGER_API AITPlayerController : public APlayerController
{
	GENERATED_BODY()
	


public:
	AITPlayerController(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UFUNCTION(BlueprintCallable, Category = "ITPlayerController")
	UITAbilitySystemComponent* GetITAbilitySystemComponent() const;

	UFUNCTION(BlueprintCallable, Category = "ITPlayerController")
	AITPlayerState* GetITPlayerState() const;

	virtual void PostProcessInput(const float DeltaTime, const bool bGamePaused) override;
};
