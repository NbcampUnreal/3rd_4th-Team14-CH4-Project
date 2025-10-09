// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/ITGameplayAbility.h"
#include "ITGameplayAbility_ItemBase.generated.h"

/**
 * 
 */
UCLASS()
class IMITATIONTRIGGER_API UITGameplayAbility_ItemBase : public UITGameplayAbility
{
	GENERATED_BODY()
	
public:
    UITGameplayAbility_ItemBase();

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item")
    TSubclassOf<class AIT_ItemBase> ItemClass;

    virtual void SpawnItem(const FVector& SpawnLocation, const FRotator& SpawnRotation);
};



