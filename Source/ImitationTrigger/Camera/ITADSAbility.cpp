#include "Camera/ITADSAbility.h"
#include "Camera/ITCameraMode.h"

UITADSAbility::UITADSAbility()
{
	ActivationOwnedTags.AddTag(ITCameraModeTags::CameraMode_ADS);
}

void UITADSAbility::InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	Super::InputReleased(Handle, ActorInfo, ActivationInfo);

	CancelAbility(Handle, ActorInfo, ActivationInfo, true);
}
