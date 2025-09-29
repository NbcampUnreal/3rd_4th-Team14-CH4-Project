// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Obstacle/ITGameplayAbilityObstacle.h"
#include "ITGameplayAbilitySpawnObstacle.generated.h"


UCLASS()
class IMITATIONTRIGGER_API UITGameplayAbilitySpawnObstacle : public UITGameplayAbilityObstacle
{
    GENERATED_BODY()

public:
    UITGameplayAbilitySpawnObstacle();

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Cooldown")
    TSubclassOf<UGameplayEffect> CooldownEffectClass;

protected:
    virtual void ActivateAbility(
        const FGameplayAbilitySpecHandle Handle,
        const FGameplayAbilityActorInfo* ActorInfo,
        const FGameplayAbilityActivationInfo ActivationInfo,
        const FGameplayEventData* TriggerEventData
    ) override;

    void SpawnObstacleActor(const FGameplayAbilityActorInfo* ActorInfo);
};
