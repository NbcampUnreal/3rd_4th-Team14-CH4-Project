

#include "Obstacle/ITGameplayAbility_SpawnObstacle.h"
#include "Obstacle/ITObstacleBase_ToyTower.h"
#include "Character/ITCharacter.h"
#include "AbilitySystem/ITAbilitySystemComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"

UITGameplayAbility_SpawnObstacle::UITGameplayAbility_SpawnObstacle()
{
    // 기본값 : 앞으로 200, 바닥 위로 약간 띄워서 스폰
    SpawnOffset = FVector(200.0f, 0.f, 0.f);
}

// 1.
//void UITGameplayAbility_SpawnObstacle::ActivateAbility(
//    const FGameplayAbilitySpecHandle Handle,
//    const FGameplayAbilityActorInfo* ActorInfo,
//    const FGameplayAbilityActivationInfo ActivationInfo,
//    const FGameplayEventData* TriggerEventData
//)
//{
//    // 반드시 서버에서만 스폰 (멀티 대비 안전)
//    if (!HasAuthority(ActivationInfo))
//    {
//        EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
//        return;
//    }
//
//    SpawnObstacle(ActorInfo);
//
//    // 쿨다운 없음 → 바로 종료
//    EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
//}

// 2.
//void UITGameplayAbility_SpawnObstacle::ActivateAbility(
//    const FGameplayAbilitySpecHandle Handle,
//    const FGameplayAbilityActorInfo* ActorInfo,
//    const FGameplayAbilityActivationInfo ActivationInfo,
//    const FGameplayEventData* TriggerEventData
//)
//{
//    // 서버에서만 동작
//    if (!HasAuthority(ActivationInfo))
//    {
//        EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
//        return;
//    }
//
//    // 쿨다운 중인지 확인
//    if (CheckCooldown(Handle, ActorInfo))
//    {
//        UE_LOG(LogTemp, Warning, TEXT("ToyTower Ability is on Cooldown!"));
//        EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
//        return;
//    }
//
//    // 장애물 소환
//    SpawnObstacleActor(ActorInfo);
//
//    // 쿨다운 부여
//    if (CooldownEffectClass)
//    {
//        ApplyGameplayEffectToOwner(
//            Handle,
//            ActorInfo,
//            ActivationInfo,
//            CooldownEffectClass.GetDefaultObject(),
//            1
//        );
//    }
//
//    EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
//}

// 2.
//void UITGameplayAbility_SpawnObstacle::ActivateAbility(
//    const FGameplayAbilitySpecHandle Handle,
//    const FGameplayAbilityActorInfo* ActorInfo,
//    const FGameplayAbilityActivationInfo ActivationInfo,
//    const FGameplayEventData* TriggerEventData)
//{
//    UE_LOG(LogTemp, Warning, TEXT("ActivateAbility called!"));
//
//    // 서버에서만 처리
//    if (!HasAuthority(ActivationInfo))
//    {
//        EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
//        return;
//    }
//
//    //// 쿨다운 확인
//    //if (CheckCooldown(Handle, ActorInfo))
//    //{
//    //    bool bOnCooldown = CheckCooldown(Handle, ActorInfo);
//    //    UE_LOG(LogTemp, Warning, TEXT("Cooldown check: %s"), bOnCooldown ? TEXT("TRUE") : TEXT("FALSE"));
//
//    //    EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
//    //    return;
//    //}
//
//    // 장애물 스폰
//    SpawnObstacleActor(ActorInfo);
//
//    // 쿨다운 적용
//    if (CooldownEffectClass)
//    {
//        ApplyGameplayEffectToOwner(
//            Handle,
//            ActorInfo,
//            ActivationInfo,
//            CooldownEffectClass.GetDefaultObject(),
//            1
//        );
//    }
//
//    EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
//}

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

    // 쿨다운 중인지 확인
    //if (CheckCooldown(Handle, ActorInfo))
    //{
    //    UE_LOG(LogTemp, Warning, TEXT("ToyTower Ability is on Cooldown!"));
    //    EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
    //    return;
    //}

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
        UE_LOG(LogTemp, Error, TEXT("ObstacleClass is NULL! Did you assign BP_ITObstacleBase_ToyTower in GA?"));
        return;
    }

    AActor* Avatar = ActorInfo->AvatarActor.Get();
    FVector SpawnLocation = Avatar->GetActorLocation() + Avatar->GetActorForwardVector() * SpawnOffset.X;
    SpawnLocation.Z += SpawnOffset.Z;

    FActorSpawnParameters SpawnParams;
    SpawnParams.Owner = Avatar;
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

    UWorld* World = Avatar->GetWorld();
    if (World)
    {
        AITObstacleBase_ToyTower* Obstacle = World->SpawnActor<AITObstacleBase_ToyTower>(ObstacleClass, SpawnLocation, Avatar->GetActorRotation(), SpawnParams);
        if (Obstacle)
        {
            UE_LOG(LogTemp, Log, TEXT("ToyTower Obstacle Spawned at %s"), *SpawnLocation.ToString());
        }
    }




}

