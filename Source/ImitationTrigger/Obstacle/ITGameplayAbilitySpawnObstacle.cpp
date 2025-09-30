

#include "Obstacle/ITGameplayAbilitySpawnObstacle.h"
#include "Obstacle/ITObstacleBase_ToyTower.h"
#include "Character/ITCharacter.h"
#include "AbilitySystem/ITAbilitySystemComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"

UITGameplayAbilitySpawnObstacle::UITGameplayAbilitySpawnObstacle()
{
    // �⺻�� : ������ 200, �ٴ� ���� �ణ ����� ����
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
//    // ���߿� ��Ʈ��ũ ������ ���� �ڵ� : UE5 GAS�� �⺻ ��Ģ : Actor ������ ���� ������ �ִ� �ʿ����� �ؾ� ��.
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
//    // ���� ���ѿ����� ����
//    //if (!HasAuthority(ActivationInfo))
//    //{
//    //    EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
//    //    return;
//    //}
//
//    // �̹� ��Ÿ�� ������ Ȯ��
//    //if (CheckCooldown(Handle, ActorInfo))
//    //{
//    //    UE_LOG(LogTemp, Warning, TEXT("ToyTower Ability on cooldown!"));
//    //    EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
//    //    return;
//    //}
//
//    // ��ֹ� ��ȯ
//    SpawnObstacleActor(ActorInfo);
//
//    // ��ٿ� GameplayEffect ����
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
//    // �̹� ��Ÿ�� ������ Ȯ��
//    if (CheckCooldown(Handle, ActorInfo))
//    {
//        EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
//        return;
//    }
//
//    SpawnObstacleActor(ActorInfo);
//
//    // ��ٿ� GameplayEffect�� ������.
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
    // �ݵ�� ���������� ���� (��Ƽ ��� ����)
    if (!HasAuthority(ActivationInfo))
    {
        EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
        return;
    }

    SpawnObstacle(ActorInfo);

    // ��ٿ� ���� �� �ٷ� ����
    EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}


void UITGameplayAbilitySpawnObstacle::SpawnObstacle(const FGameplayAbilityActorInfo* ActorInfo)
{
    if (!ObstacleClass || !ActorInfo || !ActorInfo->AvatarActor.IsValid())
    {
        UE_LOG(LogTemp, Warning, TEXT("ObstacleClass �Ǵ� ActorInfo�� ����!"));
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

