
#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/ITGameplayAbility.h"
#include "ITGameplayAbility_SuperJump.generated.h"

UCLASS()
class IMITATIONTRIGGER_API UITGameplayAbility_SuperJump : public UITGameplayAbility
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

