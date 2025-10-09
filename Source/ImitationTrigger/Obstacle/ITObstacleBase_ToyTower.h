#pragma once

#include "CoreMinimal.h"
#include "Obstacle/ITObstacleBase.h"
#include "ITObstacleBase_ToyTower.generated.h"

UCLASS()
class IMITATIONTRIGGER_API AITObstacleBase_ToyTower : public AITObstacleBase
{
	GENERATED_BODY()

public:
	AITObstacleBase_ToyTower();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

protected:
	FVector StartLocation;
	FVector TargetLocation;

	float StartTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float MoveDistance = 300.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float RiseDuration = 0.5f;

	bool bIsMoving = true;

	void ResumeMovement();

};