// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MiniMap/PlayerMiniMapComponent.h"

#include "Components/SceneCaptureComponent2D.h"
#include "Engine/TextureRenderTarget2D.h"
#include "GameFramework/Actor.h"

UPlayerMiniMapComponent::UPlayerMiniMapComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	MiniMapCapture = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("MiniMapCapture"));

	if (MiniMapCapture)
	{
		MiniMapCapture->SetupAttachment(MiniMapCapture);

		MiniMapCapture->SetRelativeLocation(FVector(0, 0, MiniMapZoom));
		

		MiniMapCapture->SetRelativeRotation(FRotator(-90, 0, 0));
		MiniMapCapture->ProjectionType = ECameraProjectionMode::Orthographic;
		MiniMapCapture->PrimitiveRenderMode = ESceneCapturePrimitiveRenderMode::PRM_RenderScenePrimitives;
	}
}

void UPlayerMiniMapComponent::TickComponent(float DeltaTime, enum ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	AActor* Owner = GetOwner();

	if (Owner)
	{
		FVector Location = Owner->GetActorLocation();
		Location.Z += MiniMapZoom;
		MiniMapCapture->SetWorldLocation(Location);
		
	}
	
}

void UPlayerMiniMapComponent::BeginPlay()
{
	Super::BeginPlay();

	if (MiniMapRenderTarget)
	{
		MiniMapCapture->TextureTarget = MiniMapRenderTarget;
	}
	
}
