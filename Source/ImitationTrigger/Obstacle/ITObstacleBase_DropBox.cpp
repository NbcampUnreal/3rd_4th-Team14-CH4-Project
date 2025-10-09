
#include "Obstacle/ITObstacleBase_DropBox.h"
#include "Components/StaticMeshComponent.h"

AITObstacleBase_DropBox::AITObstacleBase_DropBox()
{
    PrimaryActorTick.bCanEverTick = false;

    // StaticMesh를 Root로 설정 // 부모에 다른 메쉬도 존재함.
    MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
    SetRootComponent(MeshComp);

    // Physics, Collision 설정
    MeshComp->SetSimulatePhysics(true);
    MeshComp->SetEnableGravity(true);
    MeshComp->SetNotifyRigidBodyCollision(true);
    MeshComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    MeshComp->SetCollisionProfileName(TEXT("PhysicsActor"));

    MeshComp->OnComponentHit.AddDynamic(this, &AITObstacleBase_DropBox::OnHit);

}

void AITObstacleBase_DropBox::BeginPlay()
{
    Super::BeginPlay();
}

void AITObstacleBase_DropBox::OnHit(
    UPrimitiveComponent* HitComp,
    AActor* OtherActor,
    UPrimitiveComponent* OtherComp,
    FVector NormalImpulse,
    const FHitResult& Hit)
{
    if (OtherActor && OtherActor != this)
    {
        // Destroy();
    }
}