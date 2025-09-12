// Fill out your copyright notice in the Description page of Project Settings.


#include "ITTestAttributeSet.h"

#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

UITTestAttributeSet::UITTestAttributeSet()
{
}

void UITTestAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	if (Attribute == GetHealthAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxHealth());
	}
	if (Attribute == GetShieldAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxShield());
	}
	if (Attribute == GetStaminaAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxStamina());
	}
}

void UITTestAttributeSet::PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const
{
	Super::PreAttributeBaseChange(Attribute, NewValue);
}

void UITTestAttributeSet::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
{
	Super::PostAttributeChange(Attribute, OldValue, NewValue);

	if (Attribute == GetWalkSpeedAttribute())
	{
		if (ACharacter* Target = Cast<ACharacter>(GetOwningActor()))
		{
			if (UCharacterMovementComponent* MovementComponent = Target->GetCharacterMovement())
			{
				MovementComponent->MaxWalkSpeed = NewValue;
			}
		}
	}
	if (Attribute == GetJumpSpeedAttribute())
	{
		if (ACharacter* Target = Cast<ACharacter>(GetOwningActor()))
		{
			if (UCharacterMovementComponent* MovementComponent = Target->GetCharacterMovement())
			{
				MovementComponent->JumpZVelocity = NewValue;
			}
		}
	}
}

void UITTestAttributeSet::PostAttributeBaseChange(
	const FGameplayAttribute& Attribute, float OldValue, float NewValue) const
{
	Super::PostAttributeBaseChange(Attribute, OldValue, NewValue);
}

void UITTestAttributeSet::PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);
}
