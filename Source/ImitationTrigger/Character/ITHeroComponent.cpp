#include "Character/ITHeroComponent.h"
#include "Character/ITCharacter.h"
#include "Character/ITPawnData.h"
#include "EnhancedInputSubsystems.h"
#include "Input/ITInputComponent.h"
#include "Input/ITInputConfig.h"
#include "ImitationTrigger/Camera/ITCameraComponent.h"
#include "AbilitySystem/ITAbilitySystemComponent.h"
#include "System/ITGameplayTags.h"


UITHeroComponent::UITHeroComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UITHeroComponent::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	APawn* ComponentOwner = GetOwner<APawn>();
	if (IsValid(ComponentOwner))
	{
		APlayerController* PlayerController = ComponentOwner->GetController<APlayerController>();
		if (IsValid(PlayerController))
		{
			InitializePlayerInput(PlayerInputComponent, PlayerController);
		}
	}
}


AITCharacter* UITHeroComponent::GetOwnerCharacter()
{
	if (AActor* Owner = GetOwner())
	{
		return Cast<AITCharacter>(Owner);
	}
	return nullptr;
}

const AITCharacter* UITHeroComponent::GetOwnerCharacter() const
{
	if (const AActor* Owner = GetOwner())
	{
		return Cast<AITCharacter>(Owner);
	}
	return nullptr;
}

TSubclassOf<UITCameraMode> UITHeroComponent::DetermineCameraMode() const
{
	const AITCharacter* ITChar = GetOwnerCharacter();
	if (!ITChar) return nullptr;

	if (const UITPawnData* PawnData = ITChar->GetPawnData())
	{
		return PawnData->DefaultCameraMode;
	}
	return nullptr;
}

void UITHeroComponent::TryBindCameraMode()
{
	APawn* Pawn = GetOwner<APawn>();
	if (!Pawn) return;

	if (!Pawn->IsLocallyControlled()) return; // ���ø� ī�޶� ����

	// PawnData�� ���� ���� ���̸� ���⼭ ������ �� ����
	const AITCharacter* ITChar = GetOwnerCharacter();
	if (!ITChar || !ITChar->GetPawnData()) return;

	if (UITCameraComponent* Camera = UITCameraComponent::FindCameraComponent(Pawn))
	{
		if (!Camera->DetermineCameraModeDelegate.IsBound())
		{
			Camera->DetermineCameraModeDelegate.BindUObject(this, &ThisClass::DetermineCameraMode);
		}
	}
}

void UITHeroComponent::BeginPlay()
{
	Super::BeginPlay();

	// 1���� Bind�ϱ� : BeginPlay ������ �̹� Possess, PawnData�� ���� ���� ����..
	TryBindCameraMode();
}

void UITHeroComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

void UITHeroComponent::InitializePlayerInput(UInputComponent* PlayerInputComponent, APlayerController* PlayerController)
{
	AITCharacter* ITCharacter = GetOwnerCharacter();
	check(ITCharacter);

	ULocalPlayer* LocalPlayer = PlayerController->GetLocalPlayer();
	check(LocalPlayer);

	UEnhancedInputLocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	check(Subsystem);

	Subsystem->ClearAllMappings();

	if (const UITPawnData* PawnData = ITCharacter->GetPawnData())
	{
		if (const UITInputConfig* InputConfig = PawnData->InputConfig)
		{
			// Input Mapping Context ó��
			for (const FInputMappingContextAndPriority Mapping : InputConfig->DefaultMappings)
			{
				if (UInputMappingContext* IMC = Mapping.InputMapping)
				{
					FModifyContextOptions Options = {};
					Options.bIgnoreAllPressedKeysUntilRelease = false;
					Subsystem->AddMappingContext(IMC, Mapping.Priority, Options);
				}
			}

			// Bind Input Action ó��
			UITInputComponent* InputComponent = Cast<UITInputComponent>(PlayerInputComponent);
			check(InputComponent);
			
			TArray<uint32> OutBindHandles;
			InputComponent->BindAbilityActions(InputConfig, this, &ThisClass::Input_AbilityInputTagPressed, &ThisClass::Input_AbilityInputTagReleased, OutBindHandles);

			const FITGameplayTags& ITGameplayTag = FITGameplayTags::Get();
			InputComponent->BindNativeAction(InputConfig, ITGameplayTag.InputTag_Move, ETriggerEvent::Triggered, this, &ThisClass::Input_Move);
			InputComponent->BindNativeAction(InputConfig, ITGameplayTag.InputTag_Look_Mouse, ETriggerEvent::Triggered, this, &ThisClass::Input_LookMouse);
			InputComponent->BindNativeAction(InputConfig, ITGameplayTag.InputTag_Crouch, ETriggerEvent::Triggered, this, &ThisClass::Input_Crouch);
		}
	}
}

void UITHeroComponent::Input_AbilityInputTagPressed(FGameplayTag InputTag)
{
	AITCharacter* Character = GetOwnerCharacter();
	if (IsValid(Character))
	{
		UITAbilitySystemComponent* ITASC = Character->GetITAbilitySystemComponent();
		if (IsValid(ITASC))
		{
			ITASC->AbilityInputTagPressed(InputTag);
		}
	}
}

void UITHeroComponent::Input_AbilityInputTagReleased(FGameplayTag InputTag)
{
	AITCharacter* Character = GetOwnerCharacter();
	if (IsValid(Character))
	{
		UITAbilitySystemComponent* ITASC = Character->GetITAbilitySystemComponent();
		if (IsValid(ITASC))
		{
			ITASC->AbilityInputTagReleased(InputTag);
		}
	}
}

void UITHeroComponent::Input_Move(const FInputActionValue& InputActionValue)
{
	ACharacter* Character = GetOwnerCharacter();
	AController* Controller = Character ? Character->GetController() : nullptr;

	if (IsValid(Controller))
	{
		const FVector2D Value = InputActionValue.Get<FVector2D>();
		const FRotator MovementRotation(0.0f, Controller->GetControlRotation().Yaw, 0.0f);

		if (!FMath::IsNearlyZero(Value.X))
		{
			const FVector MovementDirection = MovementRotation.RotateVector(FVector::ForwardVector);
			Character->AddMovementInput(MovementDirection, Value.X);
		}
		if (!FMath::IsNearlyZero(Value.Y))
		{
			const FVector MovementDirection = MovementRotation.RotateVector(FVector::RightVector);
			Character->AddMovementInput(MovementDirection, Value.Y);
		}
	}
}

void UITHeroComponent::Input_LookMouse(const FInputActionValue& InputActionValue)
{
	ACharacter* Character = GetOwnerCharacter();

	if (IsValid(Character))
	{
		const FVector2D Value = InputActionValue.Get<FVector2D>();

		if (!FMath::IsNearlyZero(Value.X))
		{
			Character->AddControllerYawInput(Value.X);
		}
		if (!FMath::IsNearlyZero(Value.Y))
		{
			Character->AddControllerPitchInput(Value.Y);
		}
	}
}

void UITHeroComponent::Input_Crouch(const FInputActionValue& InputActionValue)
{
	ACharacter* Character = GetOwnerCharacter();

	if (IsValid(Character))
	{
		if (Character->bIsCrouched)
		{
			Character->UnCrouch();
		}
		else
		{
			Character->Crouch();
		}
	}
}


