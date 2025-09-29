// Fill out your copyright notice in the Description page of Project Settings.


#include "Obstacle/ITObstacleBase_JumpPad.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Character.h"

AITObstacleBase_JumpPad::AITObstacleBase_JumpPad()
{
    Collision = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision"));
    Collision->InitBoxExtent(FVector(100.f, 100.f, 50.f));
    Collision->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
    Collision->SetupAttachment(SceneRoot);

    Collision->OnComponentBeginOverlap.AddDynamic(this, &AITObstacleBase_JumpPad::OnOverlap);
}

void AITObstacleBase_JumpPad::OnOverlap(
    UPrimitiveComponent* OverlappedComp, 
    AActor* OtherActor,
    UPrimitiveComponent* OtherComp,
    int32 OtherBodyIndex,
    bool bFromSweep,
    const FHitResult& SweepResult)
{
    if (ACharacter* Char = Cast<ACharacter>(OtherActor))
    {
        Char->LaunchCharacter(FVector(0.f, 0.f, LaunchStrength), false, true);
        Destroy();
    }
}