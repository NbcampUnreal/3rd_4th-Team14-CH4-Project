// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/ITGameplayAbility_ItemBase.h"
#include "ITGameplayAbility_SpawnMine.generated.h"

class UNiagaraSystem;

UCLASS()
class IMITATIONTRIGGER_API UITGameplayAbility_SpawnMine : public UITGameplayAbility_ItemBase
{
	GENERATED_BODY()
	
public:
    UITGameplayAbility_SpawnMine();

protected:


    virtual void ActivateAbility(
        const FGameplayAbilitySpecHandle Handle,
        const FGameplayAbilityActorInfo* ActorInfo,
        const FGameplayAbilityActivationInfo ActivationInfo,
        const FGameplayEventData* TriggerEventData) override;
};
