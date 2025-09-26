

// 1.
//#include "Obstacle/ITObstacleBase.h"
//
//// Sets default values
//AITObstacleBase::AITObstacleBase()
//{
// 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
//	PrimaryActorTick.bCanEverTick = true;
//
//}
//
//// Called when the game starts or when spawned
//void AITObstacleBase::BeginPlay()
//{
//	Super::BeginPlay();
//	
//}
//
//// Called every frame
//void AITObstacleBase::Tick(float DeltaTime)
//{
//	Super::Tick(DeltaTime);
//
//}


#include "Obstacle/ITObstacleBase.h"

AITObstacleBase::AITObstacleBase()
{
	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	SetRootComponent(SceneRoot);

	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMeshComp->SetupAttachment(SceneRoot);



	PrimaryActorTick.bCanEverTick = true;

	RotationSpeed = 20.0f;

	RotationSpeedX = 35.0f;
	RotationSpeedY = 45.0f;
	RotationSpeedZ = 60.0f;

	// 왕복운동
	Amplitude = 100.0f;
	Speed = 2.0f;

	// 원형 회전 운동
	Radius = 200.f;

	// 지그재그 운동
	Frequency = 2.0f;


	bReplicates = true;
	SetReplicates(true);
	SetReplicateMovement(true);


}

void AITObstacleBase::BeginPlay()
{
	Super::BeginPlay();

}

void AITObstacleBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}



