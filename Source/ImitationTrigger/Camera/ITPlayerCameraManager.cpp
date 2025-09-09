

#include "Camera/ITPlayerCameraManager.h"

AITPlayerCameraManager::AITPlayerCameraManager(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	DefaultFOV = IT_CAMERA_DEFAULT_FOV;
	ViewPitchMin = IT_CAMERA_DEFAULT_PITCH_MIN;
	ViewPitchMax = IT_CAMERA_DEFAULT_PITCH_MAX;
}