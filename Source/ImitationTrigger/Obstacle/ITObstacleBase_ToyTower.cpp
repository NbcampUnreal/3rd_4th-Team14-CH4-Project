

//#include "Obstacle/ITObstacleBase_ToyTower.h"
//
//AITObstacleBase_ToyTower::AITObstacleBase_ToyTower()
//{
//	PrimaryActorTick.bCanEverTick = true;
//	MoveDistance = 200.0f;
//	bIsMoving = false;
//}
//
//void AITObstacleBase_ToyTower::BeginPlay()
//{
//	Super::BeginPlay();
//
//	StartLocation = GetActorLocation();
//	TargetLocation = StartLocation + FVector(0, 0, MoveDistance);
//	bIsMoving = true;
//}
//
//void AITObstacleBase_ToyTower::Tick(float DeltaTime)
//{
//	Super::Tick(DeltaTime);
//
//	if (bIsMoving)
//	{
//		FVector NewLoc = FMath::VInterpTo(GetActorLocation(), TargetLocation, DeltaTime, 3.0f);
//		SetActorLocation(NewLoc);
//
//		if (FVector::Dist(NewLoc, TargetLocation) < 1.f)
//		{
//			bIsMoving = false; // 목표지점 도달 시 false
//		}
//	}
//}



// 2. 왕복 운동
//#include "Obstacle/ITObstacleBase_ToyTower.h"
//
//
//
//AITObstacleBase_ToyTower::AITObstacleBase_ToyTower()
//{
//	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
//	PrimaryActorTick.bCanEverTick = true;
//
//
//	MoveDistance = 200.0f;
//	PauseDuration = 2.0f;
//	Speed = 100.0f;
//
//	bMovingUp = true;
//	bIsActive = true;
//}
//
//// Called when the game starts or when spawned
//void AITObstacleBase_ToyTower::BeginPlay()
//{
//	Super::BeginPlay();
//
//	StartLocation = GetActorLocation();
//}
//
//void AITObstacleBase_ToyTower::Tick(float DeltaTime)
//{
//	Super::Tick(DeltaTime);
//
//	if (!bIsActive)
//		return;
//
//	FVector CurrentLocation = GetActorLocation();
//	float DeltaMove = Speed * DeltaTime;
//
//	if (bMovingUp)
//	{
//		CurrentLocation.Z += DeltaMove;
//		if (CurrentLocation.Z >= StartLocation.Z + MoveDistance)
//		{
//			CurrentLocation.Z = StartLocation.Z + MoveDistance;
//			bIsActive = false;
//			bMovingUp = false;
//
//			// 일정 시간 후 다시 이동 시작
//			GetWorld()->GetTimerManager().SetTimer(
//				ResumeTimerHandle,
//				this,
//				&AITObstacleBase_ToyTower::ResumeMovement,
//				PauseDuration,
//				false
//			);
//		}
//	}
//	else
//	{
//		CurrentLocation.Z -= DeltaMove;
//		if (CurrentLocation.Z <= StartLocation.Z)
//		{
//			CurrentLocation.Z = StartLocation.Z;
//			bIsActive = false;
//			bMovingUp = true;
//
//			// 일정 시간 후 다시 이동 시작
//			GetWorld()->GetTimerManager().SetTimer(
//				ResumeTimerHandle,
//				this,
//				&AITObstacleBase_ToyTower::ResumeMovement,
//				PauseDuration,
//				false
//			);
//		}
//	}
//
//	SetActorLocation(CurrentLocation);
//}
//
//void AITObstacleBase_ToyTower::ResumeMovement()
//{
//	bIsActive = true;
//}


// 3.(1) 지면에서 올라온 뒤에 멈춤.
//#include "Obstacle/ITObstacleBase_ToyTower.h"
//
//AITObstacleBase_ToyTower::AITObstacleBase_ToyTower()
//{
//    PrimaryActorTick.bCanEverTick = true;
//    // bIsMoving, MoveDistance, Speed는 헤더 기본값 사용
//}
//
//void AITObstacleBase_ToyTower::BeginPlay()
//{
//    Super::BeginPlay();
//
//    // 베이스의 StartLocation(FVector)을 사용
//    StartLocation = GetActorLocation();
//    TargetLocation = StartLocation + FVector(0.f, 0.f, MoveDistance);
//}
//
//void AITObstacleBase_ToyTower::Tick(float DeltaTime)
//{
//    Super::Tick(DeltaTime);
//
//    if (!bIsMoving) return;
//
//    // 부드럽게 위로 이동 (Interp 속도 = Speed)
//    const FVector NewLoc = FMath::VInterpTo(GetActorLocation(), TargetLocation, DeltaTime, Speed);
//    SetActorLocation(NewLoc);
//
//    if (FVector::Dist(NewLoc, TargetLocation) < 1.f)
//    {
//        bIsMoving = false;  // 목표 도달 → 정지
//    }
//}


// 3.(2) 지면에서 올라온 뒤에 멈춤. 
#include "Obstacle/ITObstacleBase_ToyTower.h"

AITObstacleBase_ToyTower::AITObstacleBase_ToyTower()
{
	PrimaryActorTick.bCanEverTick = true;
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
