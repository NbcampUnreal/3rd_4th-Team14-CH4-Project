


#include "Obstacle/ITGameplayAbility_SpawnMeteor.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraSystem.h"
#include "GameFramework/Actor.h"
#include "TimerManager.h"

UITGameplayAbility_SpawnMeteor::UITGameplayAbility_SpawnMeteor()
{
    InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
    NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;

    bHasPlayedMeteor = false;
    bHasPlayedExplosion = false;
}

void UITGameplayAbility_SpawnMeteor::ActivateAbility(
    const FGameplayAbilitySpecHandle Handle,
    const FGameplayAbilityActorInfo* ActorInfo,
    const FGameplayAbilityActivationInfo ActivationInfo,
    const FGameplayEventData* TriggerEventData)
{
    if (!ActorInfo || !ActorInfo->AvatarActor.IsValid())
    {
        EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
        return;
    }

    AActor* Avatar = ActorInfo->AvatarActor.Get();
    UWorld* World = Avatar->GetWorld();
    if (!World)
    {
        EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
        return;
    }

    FVector StartLoc = Avatar->GetActorLocation() + FVector(0, 0, SpawnHeight);
    FVector GroundLoc = Avatar->GetActorLocation();

    // 떨어지는 Meteor VFX
    if (FallingMeteorVFX)
    {
        UNiagaraFunctionLibrary::SpawnSystemAtLocation(
            World, FallingMeteorVFX, StartLoc, FRotator::ZeroRotator);
    }

    // Meteor VFX  뒤 땅 폭발VFX // 타이머를 이용해서 딜레이를 줌.
    if (ImpactExplosionVFX) 
    {
        FTimerHandle TimerHandle;
        World->GetTimerManager().SetTimer(TimerHandle, [World, GroundLoc, this]()
            {
                UNiagaraFunctionLibrary::SpawnSystemAtLocation(
                    World, ImpactExplosionVFX, GroundLoc, FRotator::ZeroRotator);
            }, ImpactDelay, false);
    }

    EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}

//void UITGameplayAbility_SpawnMeteor::ActivateAbility(
//    const FGameplayAbilitySpecHandle Handle,
//    const FGameplayAbilityActorInfo* ActorInfo,
//    const FGameplayAbilityActivationInfo ActivationInfo,
//    const FGameplayEventData* TriggerEventData)
//{
//    if (!ActorInfo || !ActorInfo->AvatarActor.IsValid())
//    {
//        EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
//        return;
//    }
//
//    AActor* Avatar = ActorInfo->AvatarActor.Get();
//    UWorld* World = Avatar->GetWorld();
//    if (!World)
//    {
//        EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
//        return;
//    }
//
//    FVector StartLoc = Avatar->GetActorLocation() + FVector(0, 0, SpawnHeight);
//    FVector GroundLoc = Avatar->GetActorLocation();
//
//    // ===== Meteor 낙하 이펙트 (한 번만) =====
//    if (!bHasPlayedMeteor && FallingMeteorVFX)
//    {
//        UNiagaraFunctionLibrary::SpawnSystemAtLocation(
//            World,
//            FallingMeteorVFX,
//            StartLoc,
//            FRotator::ZeroRotator
//        );
//        bHasPlayedMeteor = true;
//    }
//
//    // 착지 폭발 이펙트 (딜레이 후 한 번만) 
//    if (!bHasPlayedExplosion && ImpactExplosionVFX)
//    {
//        // 안전하게 this, WeakObjectPtr 사용
//        TWeakObjectPtr<UITGameplayAbility_SpawnMeteor> WeakThis(this);
//
//        FTimerHandle TimerHandle;
//        World->GetTimerManager().SetTimer(
//            TimerHandle,
//            [World, GroundLoc, WeakThis]()
//            {
//                if (WeakThis.IsValid() && !WeakThis->bHasPlayedExplosion && WeakThis->ImpactExplosionVFX)
//                {
//                    UNiagaraFunctionLibrary::SpawnSystemAtLocation(
//                        World,
//                        WeakThis->ImpactExplosionVFX,
//                        GroundLoc,
//                        FRotator::ZeroRotator
//                    );
//                    WeakThis->bHasPlayedExplosion = true;
//                }
//            },
//            ImpactDelay,
//            false
//        );
//    }
//
//  
//    EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
//}