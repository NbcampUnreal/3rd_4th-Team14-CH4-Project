// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Obstacle/ITGameplayAbility_Obstacle.h"
#include "ITGameplayAbility_SpawnJumpPad.generated.h"

/**
 * 
 */
UCLASS()
class IMITATIONTRIGGER_API UITGameplayAbility_SpawnJumpPad : public UITGameplayAbility_Obstacle
{
	GENERATED_BODY()
	

public:
    UITGameplayAbility_SpawnJumpPad();

protected:
    virtual void ActivateAbility(
        const FGameplayAbilitySpecHandle Handle,
        const FGameplayAbilityActorInfo* ActorInfo,
        const FGameplayAbilityActivationInfo ActivationInfo,
        const FGameplayEventData* TriggerEventData
    ) override;

protected:
    //AActor* SpawnObstacleActor(const FGameplayAbilityActorInfo* ActorInfo);

};
