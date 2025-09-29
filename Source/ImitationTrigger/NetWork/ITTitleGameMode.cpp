#include "Network/ITTitleGameMode.h"
#include "Network/ITTitlePlayerController.h"


AITTitleGameMode::AITTitleGameMode()
{
	PlayerControllerClass = AITTitlePlayerController::StaticClass();
}

