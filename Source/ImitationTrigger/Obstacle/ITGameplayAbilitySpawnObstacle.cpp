

#include "Obstacle/ITGameplayAbilitySpawnObstacle.h"
#include "Obstacle/ITObstacleBase_ToyTower.h"
#include "Character/ITCharacter.h"
#include "AbilitySystem/ITAbilitySystemComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"

UITGameplayAbilitySpawnObstacle::UITGameplayAbilitySpawnObstacle()
{
    // 기본값 : 앞으로 200, 바닥 위로 약간 띄워서 스폰
    SpawnOffset = FVector(200.0f, 0.f, 0.f);
}

// 1.
//void UITGameplayAbilitySpawnObstacle::ActivateAbility(
//	const FGameplayAbilitySpecHandle Handle,
//	const FGameplayAbilityActorInfo* ActorInfo,
//	const FGameplayAbilityActivationInfo ActivationInfo,
//	const FGameplayEventData* TriggerEventData
//)
//{
//    // 나중에 네트워크 생각해 놓은 코드 : UE5 GAS의 기본 원칙 : Actor 스폰은 서버 권한이 있는 쪽에서만 해야 함.
//    //if (!HasAuthority(ActivationInfo))
//    //{
//    //    EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
//    //    return;
//    //}
//
//    SpawnObstacleActor(ActorInfo);
//
//    EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
//}

//void UITGameplayAbilitySpawnObstacle::ActivateAbility(
//    const FGameplayAbilitySpecHandle Handle,
//    const FGameplayAbilityActorInfo* ActorInfo,
//    const FGameplayAbilityActivationInfo ActivationInfo,
//    const FGameplayEventData* TriggerEventData)
//{
//    // 서버 권한에서만 실행
//    //if (!HasAuthority(ActivationInfo))
//    //{
//    //    EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
//    //    return;
//    //}
//
//    // 이미 쿨타임 중인지 확인
//    //if (CheckCooldown(Handle, ActorInfo))
//    //{
//    //    UE_LOG(LogTemp, Warning, TEXT("ToyTower Ability on cooldown!"));
//    //    EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
//    //    return;
//    //}
//
//    // 장애물 소환
//    SpawnObstacleActor(ActorInfo);
//
//    // 쿨다운 GameplayEffect 적용
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



//void UITGameplayAbilitySpawnObstacle::ActivateAbility(
//    const FGameplayAbilitySpecHandle Handle,
//    const FGameplayAbilityActorInfo* ActorInfo,
//    const FGameplayAbilityActivationInfo ActivationInfo,
//    const FGameplayEventData* TriggerEventData)
//{
//    //if (!HasAuthority(ActivationInfo))
//    //{
//    //    EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
//    //    return;
//    //}
//
//    // 이미 쿨타임 중인지 확인
//    if (CheckCooldown(Handle, ActorInfo))
//    {
//        EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
//        return;
//    }
//
//    SpawnObstacleActor(ActorInfo);
//
//    // 쿨다운 GameplayEffect를 적용함.
//    if (CooldownEffectClass)
//    {
//        ApplyGameplayEffectToOwner(Handle, ActorInfo, ActivationInfo, CooldownEffectClass.GetDefaultObject(), 1);
//    }
//
//    EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
//}


void UITGameplayAbilitySpawnObstacle::ActivateAbility(
    const FGameplayAbilitySpecHandle Handle,
    const FGameplayAbilityActorInfo* ActorInfo,
    const FGameplayAbilityActivationInfo ActivationInfo,
    const FGameplayEventData* TriggerEventData
)
{
    // 반드시 서버에서만 스폰 (멀티 대비 안전)
    if (!HasAuthority(ActivationInfo))
    {
        EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
        return;
    }

    SpawnObstacle(ActorInfo);

    // 쿨다운 없음 → 바로 종료
    EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}


void UITGameplayAbilitySpawnObstacle::SpawnObstacle(const FGameplayAbilityActorInfo* ActorInfo)
{
    if (!ObstacleClass || !ActorInfo || !ActorInfo->AvatarActor.IsValid())
    {
        UE_LOG(LogTemp, Warning, TEXT("ObstacleClass 또는 ActorInfo가 없음!"));
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

