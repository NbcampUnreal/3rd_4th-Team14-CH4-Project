#pragma once

#include "Components/ActorComponent.h"
#include "GameplayTagContainer.h"
#include "ITHeroComponent.generated.h"

class AITCharacter;
struct FInputActionValue;

/**
 * Pawn 또는 Character의 입력과 카메라 관련 기능을 담당하는 Component
 * ULyraHeroComponent을 참고하여 구현함.
 */
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class IMITATIONTRIGGER_API UITHeroComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UITHeroComponent(const FObjectInitializer& ObjectInitializer);
	void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent);

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	virtual void InitializePlayerInput(UInputComponent* PlayerInputComponent, APlayerController* PlayerController);

	void Input_AbilityInputTagPressed(FGameplayTag InputTag);
	void Input_AbilityInputTagReleased(FGameplayTag InputTag);

	void Input_Move(const FInputActionValue& InputActionValue);
	void Input_LookMouse(const FInputActionValue& InputActionValue);
	void Input_Crouch(const FInputActionValue& InputActionValue);

private:
	FORCEINLINE AITCharacter* GetOwnerCharacter();
};
