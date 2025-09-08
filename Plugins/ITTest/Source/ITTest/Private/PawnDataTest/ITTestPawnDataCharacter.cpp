#include "PawnDataTest/ITTestPawnDataCharacter.h"
#include "Character/ITPawnData.h"
#include "AbilitySystem/ITAbilitySystemComponent.h"

AITTestPawnDataCharacter::AITTestPawnDataCharacter()
{
	ASC = CreateDefaultSubobject<UITAbilitySystemComponent>("ASC");
}

void AITTestPawnDataCharacter::BeginPlay()
{
	Super::BeginPlay();

	AActor* ComponentOwner = this;
	AActor* Avatar = this;
	ASC->InitAbilityActorInfo(ComponentOwner, Avatar);

	if (IsValid(PawnData))
	{
		for (const UITAbilitySet* AbilitySet : PawnData->AbilitySets)
		{
			AbilitySet->GiveToAbilitySystem(ASC, &GrantedHandles);
		}
	}
}

UAbilitySystemComponent* AITTestPawnDataCharacter::GetAbilitySystemComponent() const
{
	return ASC;
}

UITAbilitySystemComponent* AITTestPawnDataCharacter::GetITAbilitySystemComponent() const
{
	return Cast<UITAbilitySystemComponent>(ASC);
}

void AITTestPawnDataCharacter::OnDebugKeySkill(TSubclassOf<UGameplayAbility> AbilityClass)
{
	check(ASC);

	FGameplayAbilitySpec* Spec = ASC->FindAbilitySpecFromClass(AbilityClass);
	if (Spec != nullptr)
	{
		ASC->TryActivateAbility(Spec->Handle);
	}
}

void AITTestPawnDataCharacter::OnDebugKeyX()
{
	GrantedHandles.TakeFromAbilitySystem(ASC);
}
