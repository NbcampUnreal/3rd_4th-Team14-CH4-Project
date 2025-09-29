// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Obstacle/ITGameplayAbility_Obstacle.h"
#include "ITGameplayAbility_SpawnObstacle.generated.h"


UCLASS()
class IMITATIONTRIGGER_API UITGameplayAbility_SpawnObstacle : public UITGameplayAbility_Obstacle
{
    GENERATED_BODY()

public:
    UITGameplayAbility_SpawnObstacle();

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
