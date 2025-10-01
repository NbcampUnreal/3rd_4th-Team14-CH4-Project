


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

    // �������� Meteor VFX
    if (FallingMeteorVFX)
    {
        UNiagaraFunctionLibrary::SpawnSystemAtLocation(
            World, FallingMeteorVFX, StartLoc, FRotator::ZeroRotator);
    }

    // Meteor VFX  �� �� ����VFX // Ÿ�̸Ӹ� �̿��ؼ� �����̸� ��.
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
//    // ===== Meteor ���� ����Ʈ (�� ����) =====
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
//    // ���� ���� ����Ʈ (������ �� �� ����) 
//    if (!bHasPlayedExplosion && ImpactExplosionVFX)
//    {
//        // �����ϰ� this, WeakObjectPtr ���
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