#pragma once

#include "CoreMinimal.h"
#include "Obstacle/ITGameplayAbility_Obstacle.h"
#include "Obstacle/ITObstacleBase_Meteor.h"
#include "ITGameplayAbility_SpawnMeteor.generated.h"


UCLASS()
class IMITATIONTRIGGER_API UITGameplayAbility_SpawnMeteor : public UITGameplayAbility_Obstacle
{
	GENERATED_BODY()
	
public:
    UITGameplayAbility_SpawnMeteor();

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Meteor")
    TSubclassOf<AITObstacleBase_Meteor> MeteorClass;



protected:
    virtual void ActivateAbility(
        const FGameplayAbilitySpecHandle Handle,
        const FGameplayAbilityActorInfo* ActorInfo,
        const FGameplayAbilityActivationInfo ActivationInfo,
        const FGameplayEventData* TriggerEventData
    ) override;
};
