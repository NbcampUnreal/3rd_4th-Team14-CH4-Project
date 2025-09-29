

#include "Obstacle/ITGameplayAbility_SpawnObstacle.h"
#include "Obstacle/ITObstacleBase_ToyTower.h"
#include "Character/ITCharacter.h"
#include "AbilitySystem/ITAbilitySystemComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"

UITGameplayAbility_SpawnObstacle::UITGameplayAbility_SpawnObstacle()
{
    // 기본값 : 앞으로 200, 바닥 위로 약간 띄워서 스폰
    SpawnOffset = FVector(400.0f, 0.f, 0.f);
    LifeSpan = 10.f;
}

void UITGameplayAbility_SpawnObstacle::ActivateAbility(
    const FGameplayAbilitySpecHandle Handle,
    const FGameplayAbilityActorInfo* ActorInfo,
    const FGameplayAbilityActivationInfo ActivationInfo,
    const FGameplayEventData* TriggerEventData)
{
    // 서버에서만 처리
    if (!HasAuthority(ActivationInfo))
    {
        EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
        return;
    }

    // 장애물 스폰
    SpawnObstacleActor(ActorInfo);

    // 쿨다운 적용 (Duration만 가진 GE)
    if (CooldownEffectClass)
    {
        ApplyGameplayEffectToOwner(
            Handle,
            ActorInfo,
            ActivationInfo,
            CooldownEffectClass.GetDefaultObject(), // GE_ObstacleCooldown
            1
        );
    }

    EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}



void UITGameplayAbility_SpawnObstacle::SpawnObstacleActor(const FGameplayAbilityActorInfo* ActorInfo)
{
    if (!ObstacleClass || !ActorInfo || !ActorInfo->AvatarActor.IsValid())
    {
        return;
    }

    AActor* Avatar = ActorInfo->AvatarActor.Get();
    FVector SpawnLocation = 
        Avatar->GetActorLocation() + 
        Avatar->GetActorForwardVector() * 
        SpawnOffset.X;
    
    SpawnLocation.Z += SpawnOffset.Z;

    FActorSpawnParameters SpawnParams;
    SpawnParams.Owner = Avatar;
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

    UWorld* World = Avatar->GetWorld();
    if (World)
    {
        AITObstacleBase_ToyTower* Obstacle = World->SpawnActor<AITObstacleBase_ToyTower>(ObstacleClass, SpawnLocation, Avatar->GetActorRotation(), SpawnParams);
    }
}

