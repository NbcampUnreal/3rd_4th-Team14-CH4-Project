// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Obstacle/ITObstacleBase.h"

#include "Components/SphereComponent.h" // Sphere Collider
#include "NiagaraComponent.h"       // Niagara ÄÄÆ÷³ÍÆ®

#include "ITObstacleBase_Meteor.generated.h"


/**
 * 
 */
UCLASS()
class IMITATIONTRIGGER_API AITObstacleBase_Meteor : public AITObstacleBase
{
	GENERATED_BODY()
	

public:
    AITObstacleBase_Meteor();

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    USphereComponent* Collision;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UNiagaraComponent* MeteorVFX;
};
