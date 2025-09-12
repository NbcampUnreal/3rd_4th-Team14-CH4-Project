#include "Character/ITCharacter.h"
#include "Character/ITHeroComponent.h"
#include "Character/ITPawnData.h"
#include "Net/UnrealNetwork.h"

AITCharacter::AITCharacter()
{
	PrimaryActorTick.bCanEverTick = false;

	HeroComponent = CreateDefaultSubobject<UITHeroComponent>(TEXT("HeroComponent"));
}

void AITCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void AITCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (IsValid(HeroComponent))
	{
		HeroComponent->SetupPlayerInputComponent(PlayerInputComponent);
	}
}

void AITCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, PawnData);
}

void AITCharacter::OnRep_PawnData()
{
}
