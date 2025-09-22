#include "Player/ITBattlePlayerController.h"
#include "UI/HUDWidget.h"


AITBattlePlayerController::AITBattlePlayerController()
{
}

void AITBattlePlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (IsValid(HUDWidgetClass) && HUDWidget == nullptr)
	{
		HUDWidget = CreateWidget<UHUDWidget>(this, HUDWidgetClass);
		if (IsValid(HUDWidget))
		{
			HUDWidget->AddToViewport();
		}
	}
}

void AITBattlePlayerController::EndPlay(EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	if (IsValid(HUDWidget) && HUDWidget->IsInViewport())
	{
		HUDWidget->RemoveFromParent();
	}
}
