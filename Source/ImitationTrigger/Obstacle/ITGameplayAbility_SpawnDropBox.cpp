// Fill out your copyright notice in the Description page of Project Settings.


#include "Obstacle/ITGameplayAbility_SpawnDropBox.h"
#include "Obstacle/ITObstacleBase_DropBox.h"
#include "Character/ITCharacter.h"
#include "Engine/World.h"

UITGameplayAbility_SpawnDropBox::UITGameplayAbility_SpawnDropBox()
{
    SpawnOffset = FVector(300.f, 0.f, 600.f); // 캐릭터 앞 + 위쪽 높이
    LifeSpan = 10.f;

    ObstacleClass = AITObstacleBase_DropBox::StaticClass();
}

void UITGameplayAbility_SpawnDropBox::ActivateAbility(
    const FGameplayAbilitySpecHandle Handle,
    const FGameplayAbilityActorInfo* ActorInfo,
    const FGameplayAbilityActivationInfo ActivationInfo,
    const FGameplayEventData* TriggerEventData)
{
    if (!HasAuthority(ActivationInfo))
    {
        UE_LOG(LogTemp, Warning, TEXT("SpawnDropBox: No Authority! (Client tried to spawn)"));
        EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
        return;
    }

    if (!ActorInfo || !ObstacleClass)
    {
        UE_LOG(LogTemp, Error, TEXT("SpawnDropBox: Invalid ActorInfo or ObstacleClass is NULL!"));
        return;
    }

    ACharacter* OwnerCharacter = Cast<ACharacter>(ActorInfo->AvatarActor.Get());
    if (!OwnerCharacter)
    {
        UE_LOG(LogTemp, Error, TEXT("SpawnDropBox: OwnerCharacter is NULL!"));
        return;
    }

    FVector SpawnLoc =
        OwnerCharacter->GetActorLocation() +
        OwnerCharacter->GetActorForwardVector() * SpawnOffset.X;

    SpawnLoc.Z += SpawnOffset.Z;

    FActorSpawnParameters Params;
    Params.Owner = OwnerCharacter;
    Params.Instigator = OwnerCharacter;
    Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

    UE_LOG(LogTemp, Warning, TEXT("SpawnDropBox: Trying to spawn at %s"), *SpawnLoc.ToString());

    if (UWorld* World = OwnerCharacter->GetWorld())
    {
        AActor* Obstacle = World->SpawnActor<AActor>(ObstacleClass, SpawnLoc, FRotator::ZeroRotator, Params);

        if (Obstacle)
        {
            UE_LOG(LogTemp, Warning, TEXT("SpawnDropBox: Successfully spawned %s"), *Obstacle->GetName());

            if (LifeSpan > 0.f)
            {
                Obstacle->SetLifeSpan(LifeSpan);
                UE_LOG(LogTemp, Warning, TEXT("SpawnDropBox: Set lifespan = %f"), LifeSpan);
            }

            // DropBox라면 StaticMeshComp Physics 상태 확인
            if (AITObstacleBase_DropBox* DropBox = Cast<AITObstacleBase_DropBox>(Obstacle))
            {
                if (DropBox->GetRootComponent())
                {
                    bool bSimPhysics = DropBox->GetRootComponent()->IsSimulatingPhysics();
                    UE_LOG(LogTemp, Warning, TEXT("SpawnDropBox: RootComponent Physics Enabled = %s"),
                        bSimPhysics ? TEXT("TRUE") : TEXT("FALSE"));
                }
            }
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("SpawnDropBox: FAILED to spawn actor! Class = %s"),
                *ObstacleClass->GetName());
        }
    }

    EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}