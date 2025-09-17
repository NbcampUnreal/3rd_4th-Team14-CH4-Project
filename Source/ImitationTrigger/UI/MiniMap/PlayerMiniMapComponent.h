// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlayerMiniMapComponent.generated.h"

class USceneCaptureComponent2D;
class UTextureRenderTarget2D;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class IMITATIONTRIGGER_API UPlayerMiniMapComponent : public UActorComponent
{
	GENERATED_BODY()

public:

	UPlayerMiniMapComponent();

	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	
	UPROPERTY()
	USceneCaptureComponent2D* MiniMapCapture;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MiniMap")
	UTextureRenderTarget2D* MiniMapRenderTarget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MiniMap")
	float MiniMapZoom = 1500.0f;

protected:

	virtual void BeginPlay() override;

	
private:

	
	
};