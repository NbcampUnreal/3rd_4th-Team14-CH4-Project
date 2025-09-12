#pragma once

#include "Components/ActorComponent.h"
#include "GameplayTagContainer.h"
#include "ITHeroComponent.generated.h"

class AITCharacter;
struct FInputActionValue;
class UITCameraMode;

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

	// UITCameraMode를 결정해주는 함수.
	TSubclassOf<UITCameraMode> DetermineCameraMode() const;
	// Camera에서 필요한 DetermineCameraMode해당 함수를 실행하기 만든 Get함수.
	/*AITCharacter* GetOwnerCharacter();*/ // 아래 존재
	const AITCharacter* GetOwnerCharacter() const;
	void TryBindCameraMode();


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
