// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Obstacle/ITObstacleBase.h"
#include "ITObstacleBase_DropBox.generated.h"

/**
 * 
 */
UCLASS()
class IMITATIONTRIGGER_API AITObstacleBase_DropBox : public AITObstacleBase
{
	GENERATED_BODY()

public:
    AITObstacleBase_DropBox();

protected:
    virtual void BeginPlay() override;

    UFUNCTION()
    void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor,
        UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

protected:
    // ��Ʈ���� ���� ���� StaticMesh // �θ� �ٸ� �޽�����
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UStaticMeshComponent* MeshComp;
	
};
