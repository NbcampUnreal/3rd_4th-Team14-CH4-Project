#pragma once

#include "Abilities/GameplayAbility.h"
#include "ITADSAbility.generated.h"

UCLASS()
class IMITATIONTRIGGER_API UITADSAbility : public UGameplayAbility
{
	GENERATED_BODY()
	
public:
	UITADSAbility();
	virtual void InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) override;
};
