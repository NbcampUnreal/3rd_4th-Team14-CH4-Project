#pragma once

#include "GameFramework/Actor.h"
#include "ITMinimapCapture.generated.h"

UCLASS()
class IMITATIONTRIGGER_API AITMinimapCapture : public AActor
{
	GENERATED_BODY()
	
public:
	AITMinimapCapture();
	virtual void BeginPlay() override;
	void CaptureOnce();

protected:
	void OnMapLoaded(UWorld* LoadedWorld);
};
