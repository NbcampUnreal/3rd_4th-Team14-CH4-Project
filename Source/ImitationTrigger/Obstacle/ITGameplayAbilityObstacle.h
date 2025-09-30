// Fill out your copyright notice in the Description page of Project Settings.

//#pragma once
//
//#include "CoreMinimal.h"
//#include "AbilitySystem/Abilities/ITGameplayAbility.h"
//#include "ITGameplayAbilityObstacle.generated.h"
//
///**
// * 
// */
//UCLASS()
//class IMITATIONTRIGGER_API UITGameplayAbilityObstacle : public UITGameplayAbility
//{
//	GENERATED_BODY()
//	
//};




#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/ITGameplayAbility.h"
#include "ITGameplayAbilityObstacle.generated.h"



// ��ֹ� ���� GameplayAbility ���̽�
// - SpawnObstacle, SpawnWall, SpawnTrap �� ���� ������ ����


class AITObstacleBase_ToyTower;

UCLASS(Abstract)
class IMITATIONTRIGGER_API UITGameplayAbilityObstacle : public UITGameplayAbility
{
    GENERATED_BODY()

public:
    UITGameplayAbilityObstacle();

protected:
    // ��ֹ� Ŭ���� (�ڽ� Ability���� ����)
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Obstacle")
    TSubclassOf<AActor> ObstacleClass;

    // ĳ���� ���� �� cm ���������� Offset(��Ⱚ.)���� 
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Obstacle")
    float ForwardOffset;

    // �� �� �� �ı����� ����
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Obstacle")
    float LifeSpan;

    // ��ȯ ������ (ĳ���� �� ��ġ)
    UPROPERTY(EditDefaultsOnly, Category = "Spawn")
    FVector SpawnOffset;


protected:
    // ��ֹ� ��ȯ ���� �Լ�
    AActor* SpawnObstacleActor(const FGameplayAbilityActorInfo* ActorInfo);
};