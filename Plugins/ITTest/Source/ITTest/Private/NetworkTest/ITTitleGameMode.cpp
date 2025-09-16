#include "NetworkTest/ITTitleGameMode.h"
#include "NetworkTest/ITTitlePlayerController.h"


AITTitleGameMode::AITTitleGameMode()
{
	PlayerControllerClass = AITTitlePlayerController::StaticClass();
}

