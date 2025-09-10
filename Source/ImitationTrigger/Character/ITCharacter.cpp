#include "Character/ITCharacter.h"
#include "Character/ITHeroComponent.h"
#include "Character/ITPawnData.h"
#include "Player/ITPlayerState.h"
#include "Player/ITPlayerController.h"
#include "Cosmetics/ITCharacterPartComponent.h"
#include "AbilitySystem/ITAbilitySystemComponent.h"
#include "Net/UnrealNetwork.h"

AITCharacter::AITCharacter()
{
	PrimaryActorTick.bCanEverTick = false;

	HeroComponent = CreateDefaultSubobject<UITHeroComponent>(TEXT("HeroComponent"));
}


AITPlayerController* AITCharacter::GetITPlayerController() const
{
	return GetController<AITPlayerController>();
}


AITPlayerState* AITCharacter::GetITPlayerState() const
{
	return GetPlayerState<AITPlayerState>();
}

UITAbilitySystemComponent* AITCharacter::GetITAbilitySystemComponent() const
{
	return Cast<UITAbilitySystemComponent>(GetAbilitySystemComponent());
}

UAbilitySystemComponent* AITCharacter::GetAbilitySystemComponent() const
{
	AITPlayerState* ITPlayerState = GetITPlayerState();
	if (IsValid(ITPlayerState))
	{
		return ITPlayerState->GetAbilitySystemComponent();
	}
	return nullptr;
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

	DOREPLIFETIME_CONDITION(ThisClass, PawnData, COND_InitialOnly);
}

void AITCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	SetBodyMeshes();
	AddInitCharacterPartsAtServer();
}

UITCharacterPartComponent* AITCharacter::GetITCharacterPartComponent()
{
	UActorComponent* FindComponent = GetComponentByClass(UITCharacterPartComponent::StaticClass());
	UITCharacterPartComponent* PartComponent = Cast<UITCharacterPartComponent>(FindComponent);
	return PartComponent;
}

void AITCharacter::OnRep_Owner()
{
	Super::OnRep_Owner();
	SetBodyMeshes();
}

void AITCharacter::AddInitCharacterPartsAtServer()
{
	UITCharacterPartComponent* PartComponent = GetITCharacterPartComponent();
	if (IsValid(PartComponent))
	{
		if (IsValid(PawnData))
		{
			PartComponent->AddInitCharacterParts(PawnData->InitCharacterParts);
		}
	}
}

void AITCharacter::SetBodyMeshes()
{
	UITCharacterPartComponent* PartComponent = GetITCharacterPartComponent();
	if (IsValid(PartComponent))
	{
		if (IsValid(PawnData))
		{
			PartComponent->SetBodyMeshes(PawnData->InitBodyMeshes);
		}
	}
}
