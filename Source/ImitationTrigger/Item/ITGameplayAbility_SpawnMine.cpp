// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/ITGameplayAbility_SpawnMine.h"
#include "Item/IT_ItemBase_Mine.h"

UITGameplayAbility_SpawnMine::UITGameplayAbility_SpawnMine()
{
    ItemClass = AIT_ItemBase_Mine::StaticClass();
}

void UITGameplayAbility_SpawnMine::ActivateAbility(
    const FGameplayAbilitySpecHandle Handle,
    const FGameplayAbilityActorInfo* ActorInfo,
    const FGameplayAbilityActivationInfo ActivationInfo,
    const FGameplayEventData* TriggerEventData)
{
    if (CommitAbility(Handle, ActorInfo, ActivationInfo))
    {
        AActor* Avatar = ActorInfo->AvatarActor.Get();
        if (Avatar && ItemClass)
        {
            FVector SpawnLoc = Avatar->GetActorLocation() + Avatar->GetActorForwardVector() * 200.f;
            FRotator SpawnRot = Avatar->GetActorRotation();

            SpawnItem(SpawnLoc, SpawnRot);
        }
    }

    EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}
