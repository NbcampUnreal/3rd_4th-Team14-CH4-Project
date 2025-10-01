// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/ITGameplayAbility_ItemBase.h"
#include "Item/IT_ItemBase.h"
#include "Kismet/GameplayStatics.h"

UITGameplayAbility_ItemBase::UITGameplayAbility_ItemBase()
{
    InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UITGameplayAbility_ItemBase::SpawnItem(const FVector& SpawnLocation, const FRotator& SpawnRotation)
{
    if (!ItemClass) return;

    UWorld* World = GetWorld();
    if (!World) return;

    FActorSpawnParameters SpawnParams;
    SpawnParams.Owner = GetOwningActorFromActorInfo();
    SpawnParams.Instigator = Cast<APawn>(GetOwningActorFromActorInfo());

    World->SpawnActor<AIT_ItemBase>(ItemClass, SpawnLocation, SpawnRotation, SpawnParams);
}