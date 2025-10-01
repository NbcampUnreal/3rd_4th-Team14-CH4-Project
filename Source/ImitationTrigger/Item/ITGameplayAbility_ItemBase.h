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
    // 어떤 아이템 클래스를 소환할지 정의하는 변수
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item")
    TSubclassOf<class AIT_ItemBase> ItemClass;

    // 실제 아이템을 스폰하는 함수
    virtual void SpawnItem(const FVector& SpawnLocation, const FRotator& SpawnRotation);
};
