#include "Player/ITPlayerController.h"
#include "Player/ITPlayerState.h"
#include "AbilitySystem/ITAbilitySystemComponent.h"


AITPlayerController::AITPlayerController(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

UITAbilitySystemComponent* AITPlayerController::GetITAbilitySystemComponent() const
{
	AITPlayerState* ITPlayerState = GetITPlayerState();
	if (IsValid(ITPlayerState))
	{
		return ITPlayerState->GetITAbilitySystemComponent();
	}
	return nullptr;
}

AITPlayerState* AITPlayerController::GetITPlayerState() const
{
	return GetPlayerState<AITPlayerState>();
}

void AITPlayerController::PostProcessInput(const float DeltaTime, const bool bGamePaused)
{
	UITAbilitySystemComponent* ITASC = GetITAbilitySystemComponent();
	if (IsValid(ITASC))
	{
		ITASC->ProcessAbilityInput(DeltaTime, bGamePaused);
	}

	Super::PostProcessInput(DeltaTime, bGamePaused);
}
