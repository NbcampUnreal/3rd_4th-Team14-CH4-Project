#include "Obstacle/ITGameplayAbilityObstacle.h"
#include "GameFramework/Character.h"
#include "Engine/World.h"

UITGameplayAbilityObstacle::UITGameplayAbilityObstacle()
{
    NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::ServerOnly;
    InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

    SpawnOffset = FVector(200.f, 0.f, 0.f);
}

AActor* UITGameplayAbilityObstacle::SpawnObstacleActor(const FGameplayAbilityActorInfo* ActorInfo)
{
    if (!ActorInfo || !ObstacleClass) return nullptr;

    ACharacter* OwnerCharacter = Cast<ACharacter>(ActorInfo->AvatarActor.Get());
    if (!OwnerCharacter) return nullptr;

    FVector SpawnLoc = OwnerCharacter->GetActorLocation() +
        OwnerCharacter->GetActorForwardVector() * SpawnOffset.X;
    SpawnLoc.Z += SpawnOffset.Z;

    FActorSpawnParameters Params;
    Params.Owner = OwnerCharacter;
    Params.Instigator = OwnerCharacter;
    Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

    UWorld* World = OwnerCharacter->GetWorld();
    if (!World) return nullptr;

    AActor* Obstacle = World->SpawnActor<AActor>(ObstacleClass, SpawnLoc, OwnerCharacter->GetActorRotation(), Params);
    if (Obstacle && LifeSpan > 0.f)
    {
        Obstacle->SetLifeSpan(LifeSpan);
    }

    return Obstacle;
}
