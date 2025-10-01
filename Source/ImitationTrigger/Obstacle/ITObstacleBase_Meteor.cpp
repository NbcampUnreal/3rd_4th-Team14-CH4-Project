// Fill out your copyright notice in the Description page of Project Settings.


#include "Obstacle/ITObstacleBase_Meteor.h"

AITObstacleBase_Meteor::AITObstacleBase_Meteor()
{
    Collision = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
    Collision->InitSphereRadius(100.f);
    Collision->SetCollisionProfileName(TEXT("BlockAllDynamic"));
    SetRootComponent(Collision);

    MeteorVFX = CreateDefaultSubobject<UNiagaraComponent>(TEXT("MeteorVFX"));
    MeteorVFX->SetupAttachment(Collision);

    bReplicates = true;
    SetReplicateMovement(true);
}