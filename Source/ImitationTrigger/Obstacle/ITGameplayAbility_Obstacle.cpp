#include "Obstacle/ITGameplayAbility_Obstacle.h"
#include "GameFramework/Character.h"
#include "Engine/World.h"

UITGameplayAbility_Obstacle::UITGameplayAbility_Obstacle()
{
    NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::ServerOnly;
    InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

    SpawnOffset = FVector(200.f, 0.f, 0.f);
}
