#include "Character/ITCharacter.h"
#include "Net/UnrealNetwork.h"

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

void AITCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, PawnData);
}

void AITCharacter::OnRep_PawnData()
{
}
