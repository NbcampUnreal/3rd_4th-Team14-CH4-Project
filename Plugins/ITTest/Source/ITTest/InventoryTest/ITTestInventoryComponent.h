// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ITTestInventoryComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ITTEST_API UITTestInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UITTestInventoryComponent();

protected:
	virtual void BeginPlay() override;
};
