// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/ITGameplayAbility_SpawnMine.h"
#include "Item/IT_ItemBase_Mine.h"

UITGameplayAbility_SpawnMine::UITGameplayAbility_SpawnMine()
{
    // 기본 ItemClass 지정
    ItemClass = AIT_ItemBase_Mine::StaticClass();
}

void UITGameplayAbility_SpawnMine::ActivateAbility(
    const FGameplayAbilitySpecHandle Handle,
    const FGameplayAbilityActorInfo* ActorInfo,
    const FGameplayAbilityActivationInfo ActivationInfo,
    const FGameplayEventData* TriggerEventData)
{
    if (CommitAbility(Handle, ActorInfo, ActivationInfo)) // 코스트/쿨타임 소모
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