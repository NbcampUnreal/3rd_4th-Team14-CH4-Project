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

    GrowDuration = 0.5f;
    bGrowing = false;
}

void AITObstacleBase_JumpPad::BeginPlay()
{
    Super::BeginPlay();

    SetActorScale3D(FVector(0.1f));

    StartTime = GetWorld()->GetTimeSeconds();
    bGrowing = true;
}

void AITObstacleBase_JumpPad::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (bGrowing)
    {
        float Elapsed = GetWorld()->GetTimeSeconds() - StartTime;
        float Alpha = Elapsed / GrowDuration;
        Alpha = FMath::Clamp(Alpha, 0.f, 1.f);

        FVector NewScale = FMath::Lerp(FVector(0.1f), FVector(1.0f), Alpha);
        SetActorScale3D(NewScale);

        if (Alpha >= 1.f)
        {
            bGrowing = false;
        }
    }
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