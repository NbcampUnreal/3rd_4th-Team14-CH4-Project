#include "AbilitySystem/Abilities/ITGameplayAbility.h"
#include "Character/ITCharacter.h"
#include "AbilitySystem/ITAbilitySystemComponent.h"
#include "System/ITLogChannel.h"

UITGameplayAbility::UITGameplayAbility()
{
    InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerExecution;
}

void UITGameplayAbility::ActivateAbility(
    const FGameplayAbilitySpecHandle Handle,
    const FGameplayAbilityActorInfo* ActorInfo,
    const FGameplayAbilityActivationInfo ActivationInfo,
    const FGameplayEventData* TriggerEventData
)
{
    Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UITGameplayAbility::EndAbility(
    const FGameplayAbilitySpecHandle Handle,
    const FGameplayAbilityActorInfo* ActorInfo,
    const FGameplayAbilityActivationInfo ActivationInfo,
    bool bReplicateEndAbility,
    bool bWasCancelled
)
{
    Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

AITCharacter* UITGameplayAbility::GetITCharacter(const FGameplayAbilityActorInfo* ActorInfo) const
{
    return ActorInfo ? Cast<AITCharacter>(ActorInfo->AvatarActor.Get()) : nullptr;
}

UITAbilitySystemComponent* UITGameplayAbility::GetITASC(const FGameplayAbilityActorInfo* ActorInfo) const
{
    return ActorInfo ? Cast<UITAbilitySystemComponent>(ActorInfo->AbilitySystemComponent.Get()) : nullptr;
}

bool UITGameplayAbility::HasAuthority(const FGameplayAbilityActivationInfo& ActivationInfo) const
{
    return (GetCurrentActorInfo()->IsNetAuthority());
}
