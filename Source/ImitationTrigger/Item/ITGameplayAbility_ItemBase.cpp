// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/ITGameplayAbility_ItemBase.h"
#include "Item/IT_ItemBase.h"
#include "Kismet/GameplayStatics.h"

UITGameplayAbility_ItemBase::UITGameplayAbility_ItemBase()
{
    InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
    NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::ServerOnly;
}

void UITGameplayAbility_ItemBase::SpawnItem(const FVector& SpawnLocation, const FRotator& SpawnRotation)
{
    if (!ItemClass)
    {
        return;
    }

    if (!HasAuthority(GetCurrentActivationInfoRef()))
    {
        return;
    }

    if (UWorld* World = GetWorld())
    {
        FActorSpawnParameters Params;
        Params.Owner = GetOwningActorFromActorInfo();
        Params.Instigator = Cast<APawn>(GetOwningActorFromActorInfo());
        Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

        World->SpawnActor<AIT_ItemBase>(ItemClass, SpawnLocation, SpawnRotation, Params);
    }
}

