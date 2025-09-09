// Fill out your copyright notice in the Description page of Project Settings.


#include "IT_BaseCharacter.h"
#include "AbilitySystemComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameplayEffect.h"
#include "IT_AttributeSet.h"
#include "IT_CharacterAttributeData.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"

AIT_BaseCharacter::AIT_BaseCharacter()
{
	PrimaryActorTick.bCanEverTick = false;

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("SpringArm");
	SpringArmComponent->SetupAttachment(RootComponent);
	CameraComponent = CreateDefaultSubobject<UCameraComponent>("Camera");
	CameraComponent->SetupAttachment(SpringArmComponent);
	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>("AbilitySystemComponent");
	AttributeSet = CreateDefaultSubobject<UIT_AttributeSet>("AttributeSet");

	GetCharacterMovement()->bOrientRotationToMovement = true;
	bUseControllerRotationRoll = false;
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
}

UAbilitySystemComponent* AIT_BaseCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void AIT_BaseCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem
			= ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(InputMappingContext, 0);
		}
	}
}

void AIT_BaseCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (AbilitySystemComponent)
	{
		// PlayerState 추가 후 변경
		// AbilitySystemComponent->InitAbilityActorInfo(GetPlayerState(), this);
		AbilitySystemComponent->InitAbilityActorInfo(this, this);
	}

	if (AbilitySystemComponent && JumpAbility)
	{
		FGameplayAbilitySpec JumpAbilitySpec(JumpAbility, 1, 0);
		AbilitySystemComponent->GiveAbility(JumpAbilitySpec);
	}

	InitAttributes();
}

void AIT_BaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(
			MoveAction, ETriggerEvent::Triggered, this, &AIT_BaseCharacter::ITMove);
		EnhancedInputComponent->BindAction(
			LookAction, ETriggerEvent::Triggered, this, &AIT_BaseCharacter::ITLook);
		EnhancedInputComponent->BindAction(
			JumpAction, ETriggerEvent::Started, this, &AIT_BaseCharacter::ITJump);
	}
}

void AIT_BaseCharacter::ITMove(const FInputActionValue& InputActionValue)
{
	const FVector2D MovementVector = InputActionValue.Get<FVector2D>();

	if (Controller)
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		AddMovementInput(ForwardDirection, MovementVector.X);
		AddMovementInput(RightDirection, MovementVector.Y);
	}
}

void AIT_BaseCharacter::ITLook(const FInputActionValue& InputActionValue)
{
	const FVector2D LookAxisVector = InputActionValue.Get<FVector2D>();

	if (Controller)
	{
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void AIT_BaseCharacter::ITJump(const FInputActionValue& InputActionValue)
{
	if (AbilitySystemComponent)
	{
		FGameplayTag JumpTag = FGameplayTag::RequestGameplayTag(FName("Player.Action.Jump"));
		AbilitySystemComponent->TryActivateAbilitiesByTag(FGameplayTagContainer(JumpTag));
	}
}

void AIT_BaseCharacter::InitAttributes()
{
	if (!AbilitySystemComponent || !InitAttributesData || !InitAttributesEffect)
	{
		UE_LOG(LogTemp, Error, TEXT("IT_BaseCharacter의 ASC, DA, GE가 제대로 할당되지 않았습니다."));
		return;
	}

	FGameplayEffectContextHandle EffectContextHandle = AbilitySystemComponent->MakeEffectContext();
	EffectContextHandle.AddSourceObject(this);
	FGameplayEffectSpecHandle EffectSpecHandle
		= AbilitySystemComponent->MakeOutgoingSpec(InitAttributesEffect, 1, EffectContextHandle);

	if (EffectSpecHandle.IsValid())
	{
		for (const FAttributeInitData& InitData : InitAttributesData->InitAttributes)
		{
			FString TagString = FString::Printf(TEXT("Attribute.%s"), *InitData.Attribute.GetName());
			EffectSpecHandle.Data->SetSetByCallerMagnitude(
				FGameplayTag::RequestGameplayTag(FName(*TagString)), InitData.BaseValue);
		}

		AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get());
	}
}
