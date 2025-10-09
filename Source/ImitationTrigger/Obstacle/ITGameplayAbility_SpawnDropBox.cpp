// Fill out your copyright notice in the Description page of Project Settings.


#include "Obstacle/ITGameplayAbility_SpawnDropBox.h"
#include "Obstacle/ITObstacleBase_DropBox.h"
#include "Character/ITCharacter.h"
#include "Engine/World.h"

UITGameplayAbility_SpawnDropBox::UITGameplayAbility_SpawnDropBox()
{
    SpawnOffset = FVector(300.f, 0.f, 600.f);
    LifeSpan = 10.f;
}

void UITGameplayAbility_SpawnDropBox::ActivateAbility(
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

    if (!ActorInfo || !ObstacleClass)
    {
        return;
    }

    ACharacter* OwnerCharacter = Cast<ACharacter>(ActorInfo->AvatarActor.Get());
    if (!OwnerCharacter)
    {
        return;
    }

    FVector SpawnLoc =
        OwnerCharacter->GetActorLocation() +
        OwnerCharacter->GetActorForwardVector() * SpawnOffset.X;

    SpawnLoc.Z += SpawnOffset.Z;

    FActorSpawnParameters Params;
    Params.Owner = OwnerCharacter;
    Params.Instigator = OwnerCharacter;
    Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

    if (UWorld* World = OwnerCharacter->GetWorld())
    {
        AActor* Obstacle = World->SpawnActor<AActor>(ObstacleClass, SpawnLoc, FRotator::ZeroRotator, Params);

        if (Obstacle)
        {
            if (LifeSpan > 0.f)
            {
                Obstacle->SetLifeSpan(LifeSpan);
            }
        }
    }

    EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}