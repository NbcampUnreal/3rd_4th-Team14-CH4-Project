

#include "Obstacle/ITGameplayAbility_SpawnObstacle.h"
#include "Obstacle/ITObstacleBase_ToyTower.h"
#include "Character/ITCharacter.h"
#include "AbilitySystem/ITAbilitySystemComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"

UITGameplayAbility_SpawnObstacle::UITGameplayAbility_SpawnObstacle()
{
    // �⺻�� : ������ 200, �ٴ� ���� �ణ ����� ����
    SpawnOffset = FVector(400.0f, 0.f, 0.f);
    LifeSpan = 10.f;
}

void UITGameplayAbility_SpawnObstacle::ActivateAbility(
    const FGameplayAbilitySpecHandle Handle,
    const FGameplayAbilityActorInfo* ActorInfo,
    const FGameplayAbilityActivationInfo ActivationInfo,
    const FGameplayEventData* TriggerEventData)
{
    // ���������� ó��
    if (!HasAuthority(ActivationInfo))
    {
        EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
        return;
    }

    // ��ֹ� ����
    SpawnObstacleActor(ActorInfo);

    // ��ٿ� ���� (Duration�� ���� GE)
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

