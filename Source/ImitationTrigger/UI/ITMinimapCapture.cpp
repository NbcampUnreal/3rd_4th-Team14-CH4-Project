#include "UI/ITMinimapCapture.h"
#include "Components/SceneCaptureComponent2D.h"


AITMinimapCapture::AITMinimapCapture()
{
 	PrimaryActorTick.bCanEverTick = false;
}

void AITMinimapCapture::BeginPlay()
{
	Super::BeginPlay();
	
	FCoreUObjectDelegates::PostLoadMapWithWorld.AddUObject(this, &ThisClass::OnMapLoaded);
}

void AITMinimapCapture::OnMapLoaded(UWorld* LoadedWorld)
{
	USceneCaptureComponent2D* CaptureComponent2D = FindComponentByClass<USceneCaptureComponent2D>();
	if (IsValid(CaptureComponent2D))
	{
		CaptureComponent2D->CaptureScene();
	}
}
