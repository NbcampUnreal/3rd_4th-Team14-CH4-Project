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

	// Ű�� ���� �� ȣ��Ǵ� �Լ�
	void AbilityInputTagPressed(const FGameplayTag& InputTag);

	// Ű�� �� �� ȣ��Ǵ� �Լ�
	void AbilityInputTagReleased(const FGameplayTag& InputTag);

	// �� ������(�� ƽ) ȣ��Ǵ� �Լ�, PlayerController::PostProcessInput ���� ȣ���Ѵ�.
	void ProcessAbilityInput(float DeltaTime, bool bGamePaused);

protected:
	// �̹� Frame����, ������ ��(pressed) ó���� Ability�� Handle ���
	TArray<FGameplayAbilitySpecHandle> InputPressedSpecHandles;

	// �̹� Frame����, ������ ��(released) ó���� Ability�� Handle ���
	TArray<FGameplayAbilitySpecHandle> InputReleasedSpecHandles;

	// ������ ���� ��(Held) ó���� Ability�� Handle ���
	TArray<FGameplayAbilitySpecHandle> InputHeldSpecHandles;

private:
	TArray<FGameplayAbilitySpecHandle> AbilitiesToActivate;
};
