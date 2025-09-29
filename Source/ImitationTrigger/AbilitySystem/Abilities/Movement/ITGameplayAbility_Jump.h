
#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/ITGameplayAbility.h"
#include "ITGameplayAbility_Jump.generated.h"

UCLASS()
class IMITATIONTRIGGER_API UITGameplayAbility_Jump : public UITGameplayAbility
{
	GENERATED_BODY()
	
public:
    virtual void ActivateAbility(
        const FGameplayAbilitySpecHandle Handle,
        const FGameplayAbilityActorInfo* ActorInfo,
        const FGameplayAbilityActivationInfo ActivationInfo,
        const FGameplayEventData* TriggerEventData
    ) override;
};
