// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/ITCameraMode.h"
#include "ITCameraMode_ThirdPerson.generated.h"

class UCurveVector;

UCLASS()
class IMITATIONTRIGGER_API UITCameraMode_ThirdPerson : public UITCameraMode
{
	GENERATED_BODY()

public:
	UITCameraMode_ThirdPerson(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());


	virtual void UpdateView(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly, Category = "Third Person")
	TObjectPtr<const UCurveVector> TargetOffsetCurve;
};
