
#include "Obstacle/ITObstacleBase_ToyTower.h"

AITObstacleBase_ToyTower::AITObstacleBase_ToyTower()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AITObstacleBase_ToyTower::BeginPlay()
{
	Super::BeginPlay();

	FVector CurrentLocation = GetActorLocation();
	CurrentLocation.Z -= MoveDistance;
	SetActorLocation(CurrentLocation);

	StartLocation = CurrentLocation;
	TargetLocation = CurrentLocation + FVector(0.f, 0.f, MoveDistance);

	StartTime = GetWorld()->GetTimeSeconds();
	bIsMoving = true;
}

void AITObstacleBase_ToyTower::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!bIsMoving)
	{
		return;
	}
		
	float ElapsedTime = GetWorld()->GetTimeSeconds() - StartTime;
	float Alpha = ElapsedTime / RiseDuration;

	Alpha = FMath::Clamp(Alpha, 0.f, 1.f);

	FVector NewLoc = FMath::Lerp(StartLocation, TargetLocation, Alpha);
	SetActorLocation(NewLoc);

	if (Alpha >= 1.f)
	{
		bIsMoving = false; 
	}
}
