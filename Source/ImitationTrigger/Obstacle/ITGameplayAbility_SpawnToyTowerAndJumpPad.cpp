

#include "Obstacle/ITGameplayAbility_SpawnToyTowerAndJumpPad.h"
#include "Obstacle/ITObstacleBase_ToyTower.h"
#include "Obstacle/ITObstacleBase_JumpPad.h"
#include "Character/ITCharacter.h"
#include "AbilitySystem/ITAbilitySystemComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"

UITGameplayAbility_SpawnToyTowerAndJumpPad::UITGameplayAbility_SpawnToyTowerAndJumpPad()
{
    SpawnOffset = FVector(1200.0f, 0.f, 0.f);
    LifeSpan = 10.f;
}

void UITGameplayAbility_SpawnToyTowerAndJumpPad::ActivateAbility(
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

    SpawnObstacleActors(ActorInfo);

    EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}



void UITGameplayAbility_SpawnToyTowerAndJumpPad::SpawnObstacleActors(const FGameplayAbilityActorInfo* ActorInfo)
{
    if (ObstacleClasses.Num() == 0 || !ActorInfo || !ActorInfo->AvatarActor.IsValid())
    {
        return;
    }

    AActor* Avatar = ActorInfo->AvatarActor.Get();
    UWorld* World = Avatar->GetWorld();
    if (!World)
    {
        return;
    }

    for (TSubclassOf<AActor> ObstacleClassItem : ObstacleClasses)
    {
        if (!ObstacleClassItem)
        {
            continue;
        }
            
        FVector SpawnLocation =
            Avatar->GetActorLocation() +
            Avatar->GetActorForwardVector() * SpawnOffset.X;

        SpawnLocation.Z += SpawnOffset.Z;

        if (ObstacleClassItem->IsChildOf(AITObstacleBase_JumpPad::StaticClass()))
        {
            SpawnLocation += Avatar->GetActorForwardVector() * -500.0f; 
        }

        FActorSpawnParameters SpawnParams;
        SpawnParams.Owner = Avatar;
        SpawnParams.Instigator = Cast<APawn>(Avatar);
        SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

        if (AActor* Obstacle = World->SpawnActor<AActor>(ObstacleClassItem, SpawnLocation, Avatar->GetActorRotation(), SpawnParams))
        {
            // 일정시간 뒤 삭제
            //if (LifeSpan > 0.f) 
            //{
            //    Obstacle->SetLifeSpan(LifeSpan);
            //}
        }

    }
}

