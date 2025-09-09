#pragma once

#include "GameFramework/PlayerController.h"
#include "ITPlayerController.generated.h"

UCLASS()
class IMITATIONTRIGGER_API AITPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AITPlayerController(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
};
