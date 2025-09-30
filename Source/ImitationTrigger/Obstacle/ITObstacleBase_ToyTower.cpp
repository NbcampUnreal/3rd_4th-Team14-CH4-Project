

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
//			bIsMoving = false; // ��ǥ���� ���� �� false
//		}
//	}
//}



// 2. �պ� �
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
//			// ���� �ð� �� �ٽ� �̵� ����
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
//			// ���� �ð� �� �ٽ� �̵� ����
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


// 3.(1) ���鿡�� �ö�� �ڿ� ����.
//#include "Obstacle/ITObstacleBase_ToyTower.h"
//
//AITObstacleBase_ToyTower::AITObstacleBase_ToyTower()
//{
//    PrimaryActorTick.bCanEverTick = true;
//    // bIsMoving, MoveDistance, Speed�� ��� �⺻�� ���
//}
//
//void AITObstacleBase_ToyTower::BeginPlay()
//{
//    Super::BeginPlay();
//
//    // ���̽��� StartLocation(FVector)�� ���
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
//    // �ε巴�� ���� �̵� (Interp �ӵ� = Speed)
//    const FVector NewLoc = FMath::VInterpTo(GetActorLocation(), TargetLocation, DeltaTime, Speed);
//    SetActorLocation(NewLoc);
//
//    if (FVector::Dist(NewLoc, TargetLocation) < 1.f)
//    {
//        bIsMoving = false;  // ��ǥ ���� �� ����
//    }
//}


// 3.(2) ���鿡�� �ö�� �ڿ� ����. 
#include "Obstacle/ITObstacleBase_ToyTower.h"

AITObstacleBase_ToyTower::AITObstacleBase_ToyTower()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AITObstacleBase_ToyTower::BeginPlay()
{
	Super::BeginPlay();

	// ���� ��ġ�� ���麸�� �Ʒ����� ������.
	FVector CurrentLocation = GetActorLocation();
	CurrentLocation.Z -= MoveDistance; // ���� �Ʒ��� ����
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

	// ���� ���� 0�̶� 1 ���̷� ����.
	Alpha = FMath::Clamp(Alpha, 0.f, 1.f);

	FVector NewLoc = FMath::Lerp(StartLocation, TargetLocation, Alpha);
	SetActorLocation(NewLoc);

	if (Alpha >= 1.f)
	{
		bIsMoving = false; // ��ǥ ���� ���� �� ����..
	}
}
