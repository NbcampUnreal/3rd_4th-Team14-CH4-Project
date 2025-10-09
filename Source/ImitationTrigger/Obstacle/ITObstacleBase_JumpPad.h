// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Obstacle/ITObstacleBase.h"
#include "ITObstacleBase_JumpPad.generated.h"

class UBoxComponent;

UCLASS()
class IMITATIONTRIGGER_API AITObstacleBase_JumpPad : public AITObstacleBase
{
	GENERATED_BODY()
	

public:
    AITObstacleBase_JumpPad();


protected:
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;


protected:
    UPROPERTY(VisibleAnywhere)
    UBoxComponent* Collision;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "JumpPad")
    float LaunchStrength = 2000.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visual")
    float GrowDuration;

    bool bGrowing;
    float StartTime;



    UFUNCTION()
    void OnOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
        const FHitResult& SweepResult);
};
