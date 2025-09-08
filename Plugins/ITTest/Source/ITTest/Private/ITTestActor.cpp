#include "ITTestActor.h"

AITTestActor::AITTestActor()
{
	PrimaryActorTick.bCanEverTick = false;

}

void AITTestActor::BeginPlay()
{
	Super::BeginPlay();
	
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 10, FColor::White, TEXT("ATestActor::BeginPlay()"));
	}
}
