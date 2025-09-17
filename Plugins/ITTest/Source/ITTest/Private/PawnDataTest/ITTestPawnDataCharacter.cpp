#include "PawnDataTest/ITTestPawnDataCharacter.h"
#include "Character/ITPawnData.h"
#include "AbilitySystem/ITAbilitySystemComponent.h"

AITTestPawnDataCharacter::AITTestPawnDataCharacter()
{
}

void AITTestPawnDataCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void AITTestPawnDataCharacter::OnDebugKeySkill(TSubclassOf<UGameplayAbility> AbilityClass)
{
	UAbilitySystemComponent* ASC = GetAbilitySystemComponent();
	check(ASC);

	FGameplayAbilitySpec* Spec = ASC->FindAbilitySpecFromClass(AbilityClass);
	if (Spec != nullptr)
	{
		ASC->TryActivateAbility(Spec->Handle);
	}
}

void AITTestPawnDataCharacter::OnDebugKeyX()
{
	// Ability의 제거는 PlayerState에서 처리해야 함
}
