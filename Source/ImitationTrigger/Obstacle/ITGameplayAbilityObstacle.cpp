// Fill out your copyright notice in the Description page of Project Settings.

//
//#include "Obstacle/ITGameplayAbilityObstacle.h"



#include "Obstacle/ITGameplayAbilityObstacle.h"
#include "GameFramework/Character.h"
#include "Engine/World.h"

UITGameplayAbilityObstacle::UITGameplayAbilityObstacle()
{

    NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::ServerOnly;
    InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
    ForwardOffset = 200.f;
    LifeSpan = 10.f;
}

AActor* UITGameplayAbilityObstacle::SpawnObstacleActor(const FGameplayAbilityActorInfo* ActorInfo)
{
    UE_LOG(LogTemp, Warning, TEXT("Spawning %s"), *ObstacleClass->GetName());

    if (!ActorInfo || !ObstacleClass)
    {
        return nullptr;
    }

    ACharacter* OwnerCharacter = Cast<ACharacter>(ActorInfo->AvatarActor.Get());
    if (!OwnerCharacter)
    {
        return nullptr;
    }

    FVector SpawnLoc = OwnerCharacter->GetActorLocation() +
        OwnerCharacter->GetActorForwardVector() * ForwardOffset;
    FRotator SpawnRot = FRotator::ZeroRotator;

    FActorSpawnParameters Params;
    Params.Owner = OwnerCharacter;
    Params.Instigator = OwnerCharacter;

    AActor* Obstacle = OwnerCharacter->GetWorld()->SpawnActor<AActor>(
        ObstacleClass, SpawnLoc, SpawnRot, Params);

    if (Obstacle && LifeSpan > 0.f)
    {
        Obstacle->SetLifeSpan(LifeSpan);
    }

    return Obstacle;

   

}
