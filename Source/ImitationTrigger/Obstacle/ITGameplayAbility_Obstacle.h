

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/ITGameplayAbility.h"
#include "ITGameplayAbility_Obstacle.generated.h"


UCLASS(Abstract)
class IMITATIONTRIGGER_API UITGameplayAbility_Obstacle : public UITGameplayAbility
{
    GENERATED_BODY()

public:
    UITGameplayAbility_Obstacle();

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Obstacle")
    TSubclassOf<AActor> ObstacleClass;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Obstacle")
    TArray<TSubclassOf<AActor>> ObstacleClasses;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Spawn")
    FVector SpawnOffset;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Spawn")
    float LifeSpan = 0.f;

};