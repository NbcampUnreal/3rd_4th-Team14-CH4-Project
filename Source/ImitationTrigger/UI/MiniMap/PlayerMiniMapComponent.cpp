// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MiniMap/PlayerMiniMapComponent.h"

#include "Components/SceneCaptureComponent2D.h"
#include "Engine/StaticMeshActor.h"
#include "Engine/TextureRenderTarget2D.h"
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"

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
	}
}

void UPlayerMiniMapComponent::TickComponent(float DeltaTime, enum ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	

	if (OwnerActor)
	{
		FVector Location = OwnerActor->GetActorLocation();
		Location.Z += MiniMapZoom;
		MiniMapCapture->SetWorldLocation(Location);
		
	}
	
}

void UPlayerMiniMapComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerActor = GetOwner();

	if (MiniMapRenderTarget)
	{
		MiniMapCapture->TextureTarget = MiniMapRenderTarget;
	}

	MiniMapCapture->PrimitiveRenderMode = ESceneCapturePrimitiveRenderMode::PRM_UseShowOnlyList;

	TArray<AActor*> AllStaticActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AStaticMeshActor::StaticClass(),AllStaticActors);

	for (AActor* StaticActor : AllStaticActors)
	{
		UStaticMeshComponent* StaticMeshComp = StaticActor->FindComponentByClass<UStaticMeshComponent>();

		if (StaticMeshComp)
		{
			MiniMapCapture->ShowOnlyComponents.Add(StaticMeshComp);
		}
		
	}
	
}
