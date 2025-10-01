// Fill out your copyright notice in the Description page of Project Settings.


#include "Obstacle/ITObstacleBase_DropBox.h"
#include "Components/StaticMeshComponent.h"

AITObstacleBase_DropBox::AITObstacleBase_DropBox()
{
    PrimaryActorTick.bCanEverTick = false;

    // StaticMesh�� Root�� ���� // �θ� �ٸ� �޽��� ������.
    MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
    SetRootComponent(MeshComp);

    // Physics & Collision ����
    MeshComp->SetSimulatePhysics(true);
    MeshComp->SetEnableGravity(true);
    MeshComp->SetNotifyRigidBodyCollision(true);
    MeshComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    MeshComp->SetCollisionProfileName(TEXT("PhysicsActor"));

    // ��Ʈ��ũ
    bReplicates = true;
    bAlwaysRelevant = true;
    SetReplicateMovement(true);
}

// 1.
//void AITObstacleBase_DropBox::BeginPlay()
//{
//    Super::BeginPlay();
//
//    if (USceneComponent* RootComp = GetRootComponent())
//    {
//        UE_LOG(LogTemp, Warning, TEXT("DropBox RootComponent = %s, Physics = %s"),
//            *RootComp->GetName(),
//            RootComp->IsSimulatingPhysics() ? TEXT("TRUE") : TEXT("FALSE"));
//    }
//
//    if (StaticMeshComp)
//    {
//        StaticMeshComp->OnComponentHit.AddDynamic(this, &AITObstacleBase_DropBox::OnHit);
//    }
//
//    if (StaticMeshComp)
//    {
//        StaticMeshComp->SetSimulatePhysics(true);
//        StaticMeshComp->SetEnableGravity(true);
//    }
//}

void AITObstacleBase_DropBox::BeginPlay()
{
    Super::BeginPlay();

    UE_LOG(LogTemp, Warning, TEXT("DropBox BeginPlay: Root = %s, Physics = %s"),
        *GetRootComponent()->GetName(),
        MeshComp->IsSimulatingPhysics() ? TEXT("TRUE") : TEXT("FALSE"));

    if (MeshComp)
    {
        MeshComp->OnComponentHit.AddDynamic(this, &AITObstacleBase_DropBox::OnHit);
    }
}

void AITObstacleBase_DropBox::OnHit(
    UPrimitiveComponent* HitComp,
    AActor* OtherActor,
    UPrimitiveComponent* OtherComp,
    FVector NormalImpulse,
    const FHitResult& Hit)
{
    // ���� ������ ����.
    if (OtherActor && OtherActor != this)
    {
        /*Destroy();*/ 
    }
}