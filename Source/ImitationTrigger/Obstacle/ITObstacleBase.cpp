
#include "Obstacle/ITObstacleBase.h"

AITObstacleBase::AITObstacleBase()
{
	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	SetRootComponent(SceneRoot);

	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMeshComp->SetupAttachment(SceneRoot);


	PrimaryActorTick.bCanEverTick = true;

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



