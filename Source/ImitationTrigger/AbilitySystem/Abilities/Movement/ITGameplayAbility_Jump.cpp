
#include "AbilitySystem/Abilities/Movement/ITGameplayAbility_Jump.h"
#include "Character/ITCharacter.h"

void UITGameplayAbility_Jump::ActivateAbility(
    const FGameplayAbilitySpecHandle Handle,
    const FGameplayAbilityActorInfo* ActorInfo,
    const FGameplayAbilityActivationInfo ActivationInfo,
    const FGameplayEventData* TriggerEventData)
{
    if (AITCharacter* Character = GetITCharacter(ActorInfo))
    {
        Character->Jump();
    }

    EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}