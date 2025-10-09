
#include "Obstacle/ITGameplayAbility_SpawnJumpPad.h"
#include "Obstacle/ITObstacleBase_JumpPad.h"
#include "Character/ITCharacter.h"

UITGameplayAbility_SpawnJumpPad::UITGameplayAbility_SpawnJumpPad()
{
    SpawnOffset = FVector(100.f, 0.f, 0.f);
    LifeSpan = 10.f;
}

void UITGameplayAbility_SpawnJumpPad::ActivateAbility(
    const FGameplayAbilitySpecHandle Handle,
    const FGameplayAbilityActorInfo* ActorInfo,
    const FGameplayAbilityActivationInfo ActivationInfo,
    const FGameplayEventData* TriggerEventData)
{
    if (!HasAuthority(ActivationInfo))
    {
        EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
        return;
    }

    EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}

