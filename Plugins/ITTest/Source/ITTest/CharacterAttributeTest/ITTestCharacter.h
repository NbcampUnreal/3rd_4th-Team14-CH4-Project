// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "Abilities/GameplayAbility.h"
#include "ITTestCharacter.generated.h"

class UITCharacterAttributeData;
struct FInputActionValue;
class UInputAction;
class UInputMappingContext;
class UCameraComponent;
class USpringArmComponent;
class UITTestAttributeSet;

UCLASS()
class ITTEST_API AITTestCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AITTestCharacter();

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

protected:
	virtual void BeginPlay() override;
	virtual void PossessedBy(AController* NewController) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Component
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "IT|Component")
	TObjectPtr<USpringArmComponent> SpringArmComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "IT|Component")
	TObjectPtr<UCameraComponent> CameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "IT|Component")
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "IT|Component")
	TObjectPtr<UITTestAttributeSet> AttributeSet;

	// Attribute
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "IT|Attribute")
	TObjectPtr<UITCharacterAttributeData> InitAttributesData;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "IT|Attribute")
	TSubclassOf<UGameplayEffect> InitAttributesEffect;

	// Input
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "IT|Input")
	TObjectPtr<UInputMappingContext> InputMappingContext;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "IT|Input")
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "IT|Input")
	TObjectPtr<UInputAction> LookAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "IT|Input")
	TObjectPtr<UInputAction> JumpAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "IT|Input|Ability")
	TSubclassOf<UGameplayAbility> JumpAbility;

private:
	void ITMove(const FInputActionValue& InputActionValue);
	void ITLook(const FInputActionValue& InputActionValue);
	void ITJump(const FInputActionValue& InputActionValue);

	void InitAttributes();
};
