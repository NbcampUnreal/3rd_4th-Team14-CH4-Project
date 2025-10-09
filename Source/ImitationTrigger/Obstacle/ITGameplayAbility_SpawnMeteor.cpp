


#include "Obstacle/ITGameplayAbility_SpawnMeteor.h"
#include "NiagaraFunctionLibrary.h"
#include "GameFramework/Actor.h"
#include "TimerManager.h"

UITGameplayAbility_SpawnMeteor::UITGameplayAbility_SpawnMeteor()
{
    SpawnOffset = FVector(200.f, 0.f, 0.f);
}


void UITGameplayAbility_SpawnMeteor::ActivateAbility(
    const FGameplayAbilitySpecHandle Handle,
    const FGameplayAbilityActorInfo* ActorInfo,
    const FGameplayAbilityActivationInfo ActivationInfo,
    const FGameplayEventData* TriggerEventData
)
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

    FVector ForwardAdjustedOffset = Avatar->GetActorRotation().RotateVector(SpawnOffset);
    FVector SpawnLoc = Avatar->GetActorLocation()
        + Avatar->GetActorForwardVector() * 500.f;
    FRotator SpawnRot = FRotator::ZeroRotator;

    FActorSpawnParameters Params;
    Params.Owner = Avatar;
    Params.Instigator = Cast<APawn>(Avatar);
    Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

    AITObstacleBase_Meteor* Meteor = World->SpawnActor<AITObstacleBase_Meteor>(MeteorClass, SpawnLoc, SpawnRot, Params);

    EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}


