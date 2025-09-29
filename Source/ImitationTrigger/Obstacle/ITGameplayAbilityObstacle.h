

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/ITGameplayAbility.h"
#include "ITGameplayAbilityObstacle.generated.h"


UCLASS(Abstract)
class IMITATIONTRIGGER_API UITGameplayAbilityObstacle : public UITGameplayAbility
{
    GENERATED_BODY()

public:
    UITGameplayAbilityObstacle();

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Obstacle")
    TSubclassOf<AActor> ObstacleClass;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Spawn")
    FVector SpawnOffset;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Spawn")
    float LifeSpan = 10.f;

protected:
    AActor* SpawnObstacleActor(const FGameplayAbilityActorInfo* ActorInfo);
};