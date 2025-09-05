#include "Character/ITCharacter.h"

AITCharacter::AITCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AITCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void AITCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AITCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}
