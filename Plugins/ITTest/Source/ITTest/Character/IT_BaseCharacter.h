// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "Abilities/GameplayAbility.h"
#include "IT_BaseCharacter.generated.h"

struct FInputActionValue;
class UInputAction;
class UInputMappingContext;
class UCameraComponent;
class USpringArmComponent;
class UIT_AttributeSet;

UCLASS()
class ITTEST_API AIT_BaseCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AIT_BaseCharacter();

	// ASC를 사용하기 위해 필수적으로 구현해야하는 함수
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

protected:
	virtual void BeginPlay() override;
	virtual void PossessedBy(AController* NewController) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Component
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "IT")
	TObjectPtr<USpringArmComponent> SpringArmComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "IT")
	TObjectPtr<UCameraComponent> CameraComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "IT")
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "IT")
	TObjectPtr<UIT_AttributeSet> AttributeSet;

	// Input
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputMappingContext> InputMappingContext;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> MoveAction;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> LookAction;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> JumpAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability")
	TSubclassOf<UGameplayAbility> JumpAbility;

private:
	void ITMove(const FInputActionValue& InputActionValue);
	void ITLook(const FInputActionValue& InputActionValue);
	void ITJump(const FInputActionValue& InputActionValue);
};
