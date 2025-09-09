#pragma once

#include "AbilitySystemComponent.h"
#include "ITAbilitySystemComponent.generated.h"

UCLASS()
class IMITATIONTRIGGER_API UITAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	UITAbilitySystemComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	void ClearAbilityInput();

	// 키를 누를 때 호출되는 함수
	void AbilityInputTagPressed(const FGameplayTag& InputTag);

	// 키를 뗄 때 호출되는 함수
	void AbilityInputTagReleased(const FGameplayTag& InputTag);

	// 매 프레임(매 틱) 호출되는 함수, PlayerController::PostProcessInput 에서 호출한다.
	void ProcessAbilityInput(float DeltaTime, bool bGamePaused);

protected:
	// 이번 Frame에서, 눌렀을 때(pressed) 처리할 Ability의 Handle 목록
	TArray<FGameplayAbilitySpecHandle> InputPressedSpecHandles;

	// 이번 Frame에서, 떼었을 때(released) 처리할 Ability의 Handle 목록
	TArray<FGameplayAbilitySpecHandle> InputReleasedSpecHandles;

	// 누르고 있을 때(Held) 처리할 Ability의 Handle 목록
	TArray<FGameplayAbilitySpecHandle> InputHeldSpecHandles;

private:
	TArray<FGameplayAbilitySpecHandle> AbilitiesToActivate;
};
