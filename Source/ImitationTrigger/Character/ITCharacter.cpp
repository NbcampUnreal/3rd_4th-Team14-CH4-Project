#include "Character/ITCharacter.h"
#include "Character/ITHeroComponent.h"
#include "Character/ITPawnData.h"
#include "Player/ITPlayerState.h"
#include "Player/ITPlayerController.h"
#include "Camera/ITCameraComponent.h"
#include "Cosmetics/ITCharacterPartComponent.h"
#include "Cosmetics/ITCharacterAnimComponent.h"
#include "AbilitySystem/ITAbilitySystemComponent.h"
#include "Net/UnrealNetwork.h"
#include "ITPawnDataList.h"

AITCharacter::AITCharacter()
{
	PrimaryActorTick.bCanEverTick = false;

	HeroComponent = CreateDefaultSubobject<UITHeroComponent>(TEXT("HeroComponent"));

	CameraComponent = CreateDefaultSubobject<UITCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetRelativeLocation(FVector(-300.0f, 0.0f, 75.0f));
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
	if (!HasAuthority())
	{
		SetBodyMeshes();
	}
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

	DOREPLIFETIME_CONDITION(ThisClass, PawnData, COND_None);
	DOREPLIFETIME_CONDITION(ThisClass, PawnDataList, COND_None);
}

void AITCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	SetBodyMeshes();
	SetAnimLayerRules();
	AddInitCharacterPartsAtServer();
}

FGameplayTagContainer AITCharacter::GetASCGameplayTags() const
{
	UAbilitySystemComponent* ASC = this->GetAbilitySystemComponent();
	if (IsValid(ASC))
	{
		return ASC->GetOwnedGameplayTags();
	}
	else
	{
		return FGameplayTagContainer();
	}
}

UITCharacterPartComponent* AITCharacter::GetITCharacterPartComponent()
{
	UActorComponent* FindComponent = GetComponentByClass(UITCharacterPartComponent::StaticClass());
	UITCharacterPartComponent* PartComponent = Cast<UITCharacterPartComponent>(FindComponent);
	return PartComponent;
}

UITCharacterAnimComponent* AITCharacter::GetITCharacterAnimComponent()
{
	UActorComponent* FindComponent = GetComponentByClass(UITCharacterAnimComponent::StaticClass());
	UITCharacterAnimComponent* AnimComponent = Cast<UITCharacterAnimComponent>(FindComponent);
	return AnimComponent;
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

void AITCharacter::SetAnimLayerRules()
{
	UITCharacterAnimComponent* PartComponent = GetITCharacterAnimComponent();
	if (IsValid(PartComponent))
	{
		if (IsValid(PawnData))
		{
			PartComponent->SetAnimLayerRules(PawnData->AnimLayerRules);
		}
	}
}

void AITCharacter::SetPawnDataByIndex(int32 Index)
{
	if (HasAuthority())
	{
		if (IsValid(PawnDataList)) 
		{
			if (PawnDataList->PawnDatas.IsValidIndex(Index))
			{
				PawnData = PawnDataList->PawnDatas[Index];
				UE_LOG(LogTemp, Warning, TEXT("@@@@SetPawnDataByIndex: %d@@@@"), Index);
			}
		}
	}
}
