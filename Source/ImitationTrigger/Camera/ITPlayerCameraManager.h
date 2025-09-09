#pragma once

#include "CoreMinimal.h"
#include "Camera/PlayerCameraManager.h"
#include "ITPlayerCameraManager.generated.h"

#define IT_CAMERA_DEFAULT_FOV (80.0f)
#define IT_CAMERA_DEFAULT_PITCH_MIN (-89.0f)
#define IT_CAMERA_DEFAULT_PITCH_MAX (89.0f)

UCLASS()
class IMITATIONTRIGGER_API AITPlayerCameraManager : public APlayerCameraManager
{
	GENERATED_BODY()

public:
	AITPlayerCameraManager(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

};
