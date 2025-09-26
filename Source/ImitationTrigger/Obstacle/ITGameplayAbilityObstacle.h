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



// 장애물 전용 GameplayAbility 베이스
// - SpawnObstacle, SpawnWall, SpawnTrap 등 공통 로직을 포함


class AITObstacleBase_ToyTower;

UCLASS(Abstract)
class IMITATIONTRIGGER_API UITGameplayAbilityObstacle : public UITGameplayAbility
{
    GENERATED_BODY()

public:
    UITGameplayAbilityObstacle();

protected:
    // 장애물 클래스 (자식 Ability에서 설정)
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Obstacle")
    TSubclassOf<AActor> ObstacleClass;

    // 캐릭터 앞쪽 몇 cm 있을것인지 Offset(상쇄값.)설정 
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Obstacle")
    float ForwardOffset;

    // 몇 초 후 파괴할지 결정
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Obstacle")
    float LifeSpan;

    // 소환 오프셋 (캐릭터 앞 위치)
    UPROPERTY(EditDefaultsOnly, Category = "Spawn")
    FVector SpawnOffset;


protected:
    // 장애물 소환 공통 함수
    AActor* SpawnObstacleActor(const FGameplayAbilityActorInfo* ActorInfo);
};