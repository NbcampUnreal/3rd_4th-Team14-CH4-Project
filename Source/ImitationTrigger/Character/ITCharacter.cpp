#include "Character/ITCharacter.h"
#include "Character/ITHeroComponent.h"
#include "Character/ITPawnData.h"
#include "Player/ITPlayerState.h"
#include "Player/ITPlayerController.h"
#include "Camera/ITCameraComponent.h"
#include "Cosmetics/ITCharacterPartComponent.h"
#include "Cosmetics/ITCharacterAnimComponent.h"
#include "AbilitySystem/ITAbilitySystemComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Net/UnrealNetwork.h"

AITCharacter::AITCharacter()
{
	PrimaryActorTick.bCanEverTick = false;

	HeroComponent = CreateDefaultSubobject<UITHeroComponent>(TEXT("HeroComponent"));

	CameraComponent = CreateDefaultSubobject<UITCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetRelativeLocation(FVector(-300.0f, 0.0f, 75.0f));
}

void AITCharacter::MulticastRPC_PlayFireEffects_Implementation(
	UAnimMontage* ReboundAnimMontage, UAnimMontage* FireAnimMontage, USkeleton* MatchedSkeleton)
{
	if (!ReboundAnimMontage || !FireAnimMontage || !MatchedSkeleton)
	{
		return;
	}

	PlayAnimMontage(ReboundAnimMontage);

	TArray<UChildActorComponent*> ChildActorComponents;
	GetComponents<UChildActorComponent>(ChildActorComponents);
	for (UChildActorComponent* ChildActorComponent : ChildActorComponents)
	{
		if (ChildActorComponent && ChildActorComponent->GetChildActor())
		{
			USkeletalMeshComponent* WeaponSkeletalMeshComponent
				= ChildActorComponent->GetChildActor()->FindComponentByClass<USkeletalMeshComponent>();

			if (WeaponSkeletalMeshComponent && WeaponSkeletalMeshComponent->GetSkeletalMeshAsset())
			{
				if (WeaponSkeletalMeshComponent->GetSkeletalMeshAsset()->GetSkeleton() == MatchedSkeleton)
				{
					// ABP 없이 바로 재생 가능, 하지만 기존의 몽타주를 그대로 사용
					WeaponSkeletalMeshComponent->PlayAnimation(FireAnimMontage, false);
					break;
				}
			}
		}
	}
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

	DOREPLIFETIME_CONDITION(ThisClass, PawnData, COND_InitialOnly);
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

void AITCharacter::MulticastRPC_OnDead_Implementation()
{
	if (HasAuthority())
	{
		SetLifeSpan(3.0f);

		UCharacterMovementComponent* MovementComponent = GetCharacterMovement();
		if (IsValid(MovementComponent))
		{
			MovementComponent->DisableMovement();
		}
	}
	else
	{
		APlayerController* PC = Cast<APlayerController>(GetController());
		if (IsValid(PC))
		{
			DisableInput(PC);
		}

		USkeletalMeshComponent* RootMeshComponent = GetMesh();
		if (IsValid(RootMeshComponent))
		{
			RootMeshComponent->SetCollisionResponseToChannel(ECC_Visibility, ECR_Ignore);
			RootMeshComponent->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
			RootMeshComponent->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);
		}

		TArray<UChildActorComponent*> ChildActorComponents;
		GetComponents<UChildActorComponent>(ChildActorComponents);
		for (UChildActorComponent* ChildActorComponent : ChildActorComponents)
		{
			if (IsValid(ChildActorComponent))
			{
				AActor* ChildActor = ChildActorComponent->GetChildActor();
				if (IsValid(ChildActor))
				{
					USkeletalMeshComponent* MeshComponent = ChildActor->FindComponentByClass<USkeletalMeshComponent>();

					if (IsValid(MeshComponent))
					{
						MeshComponent->SetSimulatePhysics(true);
						MeshComponent->SetCollisionProfileName(FName(TEXT("Ragdoll")));
						MeshComponent->SetCollisionResponseToChannel(ECC_Visibility, ECR_Ignore);
						MeshComponent->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
					}
				}
			}
		}
	}
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
