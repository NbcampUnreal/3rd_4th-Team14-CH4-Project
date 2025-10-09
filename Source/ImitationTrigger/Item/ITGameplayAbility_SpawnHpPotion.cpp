#include "Item/ITGameplayAbility_SpawnHpPotion.h"
#include "Item/IT_ItemBase_HpPotion.h"

UITGameplayAbility_SpawnHpPotion::UITGameplayAbility_SpawnHpPotion()
{
    ItemClass = AIT_ItemBase_HpPotion::StaticClass();
}

void UITGameplayAbility_SpawnHpPotion::ActivateAbility(
    const FGameplayAbilitySpecHandle Handle,
    const FGameplayAbilityActorInfo* ActorInfo,
    const FGameplayAbilityActivationInfo ActivationInfo,
    const FGameplayEventData* TriggerEventData)
{
    if (CommitAbility(Handle, ActorInfo, ActivationInfo))
    {
        AActor* Avatar = ActorInfo->AvatarActor.Get();
        if (Avatar)
        {
            FVector SpawnLoc = Avatar->GetActorLocation() + Avatar->GetActorForwardVector() * 200.f;
            FRotator SpawnRot = Avatar->GetActorRotation();

            SpawnItem(SpawnLoc, SpawnRot);
        }
    }

    EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}