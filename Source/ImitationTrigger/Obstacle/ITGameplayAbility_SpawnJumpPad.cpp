// Fill out your copyright notice in the Description page of Project Settings.


#include "Obstacle/ITGameplayAbility_SpawnJumpPad.h"
#include "Obstacle/ITObstacleBase_JumpPad.h"
#include "Character/ITCharacter.h"

UITGameplayAbility_SpawnJumpPad::UITGameplayAbility_SpawnJumpPad()
{
    SpawnOffset = FVector(300.f, 0.f, 0.f);
    LifeSpan = 10.f;

    //ObstacleClass = AITObstacleBase_JumpPad::StaticClass();
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

    //SpawnObstacleActor(ActorInfo);

    EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}

//AActor* UITGameplayAbility_SpawnJumpPad::SpawnObstacleActor(const FGameplayAbilityActorInfo* ActorInfo)
//{
//    if (!ActorInfo || !ObstacleClass)
//    {
//        return nullptr;
//    }
//
//    ACharacter* OwnerCharacter = Cast<ACharacter>(ActorInfo->AvatarActor.Get());
//    if (!OwnerCharacter)
//    {
//        return nullptr;
//    }
//
//    FVector SpawnLoc = 
//        OwnerCharacter->GetActorLocation() +
//        OwnerCharacter->GetActorForwardVector() *
//        SpawnOffset.X;
//
//        SpawnLoc.Z += SpawnOffset.Z;
//
//    FActorSpawnParameters Params;
//    Params.Owner = OwnerCharacter;
//    Params.Instigator = OwnerCharacter;
//    Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
//
//    if (UWorld* World = OwnerCharacter->GetWorld())
//    {
//        if (AActor* Obstacle = World->SpawnActor<AActor>(ObstacleClass, SpawnLoc, OwnerCharacter->GetActorRotation(), Params))
//        {
//            if (LifeSpan > 0.f)
//            {
//                Obstacle->SetLifeSpan(LifeSpan);
//            }
//            return Obstacle;
//        }
//    }
//    return nullptr;
//}