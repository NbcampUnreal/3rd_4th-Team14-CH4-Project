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
    UNiagaraSystem* FallingMeteorVFX;   // �ϴÿ��� �������� ����Ʈ

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "VFX")
    UNiagaraSystem* ImpactExplosionVFX; // ���� �� ���� ����Ʈ

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "VFX")
    float SpawnHeight = 800.f;          // ĳ���� �� ����

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "VFX")
    float ImpactDelay = 5.0f;           // �� �� �� ���� FX �������� (Meteor �̵� �ð�)

protected:
    virtual void ActivateAbility(
        const FGameplayAbilitySpecHandle Handle,
        const FGameplayAbilityActorInfo* ActorInfo,
        const FGameplayAbilityActivationInfo ActivationInfo,
        const FGameplayEventData* TriggerEventData
    ) override;
};
