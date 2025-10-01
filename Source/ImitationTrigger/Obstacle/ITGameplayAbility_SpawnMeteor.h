// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Obstacle/ITGameplayAbility_Obstacle.h"
#include "ITGameplayAbility_SpawnMeteor.generated.h"


class UNiagaraSystem;

UCLASS()
class IMITATIONTRIGGER_API UITGameplayAbility_SpawnMeteor : public UITGameplayAbility_Obstacle
{
	GENERATED_BODY()
	
public:
    UITGameplayAbility_SpawnMeteor();

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "VFX")
    UNiagaraSystem* FallingMeteorVFX;   // 하늘에서 떨어지는 이펙트

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "VFX")
    UNiagaraSystem* ImpactExplosionVFX; // 착지 시 폭발 이펙트

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "VFX")
    float SpawnHeight = 800.f;          // 캐릭터 위 높이

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "VFX")
    float ImpactDelay = 5.0f;           // 몇 초 뒤 폭발 FX 보여줄지 (Meteor 이동 시간)

protected:
    virtual void ActivateAbility(
        const FGameplayAbilitySpecHandle Handle,
        const FGameplayAbilityActorInfo* ActorInfo,
        const FGameplayAbilityActivationInfo ActivationInfo,
        const FGameplayEventData* TriggerEventData
    ) override;
};
