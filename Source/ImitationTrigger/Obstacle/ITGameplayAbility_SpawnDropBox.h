
#pragma once

#include "CoreMinimal.h"
#include "Obstacle/ITGameplayAbility_Obstacle.h"
#include "ITGameplayAbility_SpawnDropBox.generated.h"


UCLASS()
class IMITATIONTRIGGER_API UITGameplayAbility_SpawnDropBox : public UITGameplayAbility_Obstacle
{
	GENERATED_BODY()

public:
    UITGameplayAbility_SpawnDropBox();

protected:
    virtual void ActivateAbility(
        const FGameplayAbilitySpecHandle Handle,
        const FGameplayAbilityActorInfo* ActorInfo,
        const FGameplayAbilityActivationInfo ActivationInfo,
        const FGameplayEventData* TriggerEventData
    ) override;

protected:
    AActor* SpawnObstacleActor(const FGameplayAbilityActorInfo* ActorInfo);
	
};
