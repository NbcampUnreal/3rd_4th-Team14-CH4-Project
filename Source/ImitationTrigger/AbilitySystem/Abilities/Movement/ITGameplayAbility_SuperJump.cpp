
#include "AbilitySystem/Abilities/Movement/ITGameplayAbility_SuperJump.h"
#include "Character/ITCharacter.h"

void UITGameplayAbility_SuperJump::ActivateAbility(
    const FGameplayAbilitySpecHandle Handle,
    const FGameplayAbilityActorInfo* ActorInfo,
    const FGameplayAbilityActivationInfo ActivationInfo,
    const FGameplayEventData* TriggerEventData)
{
    if (AITCharacter* Character = GetITCharacter(ActorInfo))
    {
        // ½´ÆÛÁ¡ÇÁ
        Character->LaunchCharacter(FVector(0.f, 0.f, 750.f), false, true);
    }

    EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}
