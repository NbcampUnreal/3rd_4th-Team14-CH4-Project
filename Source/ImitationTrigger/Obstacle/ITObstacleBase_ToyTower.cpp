

// 3.(2) 지면에서 올라온 뒤에 멈춤. 
#include "Obstacle/ITObstacleBase_ToyTower.h"

AITObstacleBase_ToyTower::AITObstacleBase_ToyTower()
{
	PrimaryActorTick.bCanEverTick = true;


	// 상속하고 있는 ITObstacleBase클래스에서 구현.
	//bReplicates = true;
	//SetReplicateMovement(true);
	//PrimaryActorTick.bCanEverTick = true;
}

void AITObstacleBase_ToyTower::BeginPlay()
{
	Super::BeginPlay();

	// 스폰 위치는 지면보다 아래에서 시작함.
	FVector CurrentLocation = GetActorLocation();
	CurrentLocation.Z -= MoveDistance; // 지면 아래서 시작
	SetActorLocation(CurrentLocation);

	StartLocation = CurrentLocation;
	TargetLocation = CurrentLocation + FVector(0.f, 0.f, MoveDistance);

	StartTime = GetWorld()->GetTimeSeconds();
	bIsMoving = true;
}

void AITObstacleBase_ToyTower::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!bIsMoving) return;

	float ElapsedTime = GetWorld()->GetTimeSeconds() - StartTime;
	float Alpha = ElapsedTime / RiseDuration;

	// 선형 보간 0이랑 1 사이로 정함.
	Alpha = FMath::Clamp(Alpha, 0.f, 1.f);

	FVector NewLoc = FMath::Lerp(StartLocation, TargetLocation, Alpha);
	SetActorLocation(NewLoc);

	if (Alpha >= 1.f)
	{
		bIsMoving = false; // 목표 지점 도달 후 멈춤..
	}
}
