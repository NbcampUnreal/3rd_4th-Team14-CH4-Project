// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ITTestEquipmentComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ITTEST_API UITTestEquipmentComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UITTestEquipmentComponent();

protected:
	virtual void BeginPlay() override;
};
