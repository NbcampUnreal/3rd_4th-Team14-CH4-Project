#include "AbilitySystem/ITAbilitySet.h"
#include "AbilitySystem/ITAbilitySystemComponent.h"

void FITAbilitySet_GrantedHandles::AddAbilitySpecHandle(const FGameplayAbilitySpecHandle& Handle)
{
	if (Handle.IsValid())
	{
		AbilitySpecHandles.Add(Handle);
	}
}

void FITAbilitySet_GrantedHandles::AddGameplayEffectHandle(const FActiveGameplayEffectHandle& Handle)
{
	if (Handle.IsValid())
	{
		GameplayEffectHandles.Add(Handle);
	}
}

void FITAbilitySet_GrantedHandles::AddAttributeSet(UAttributeSet* Set)
{
	if (Set)
	{
		GrantedAttributeSets.Add(Set);
	}
}

void FITAbilitySet_GrantedHandles::TakeFromAbilitySystem(UITAbilitySystemComponent* ITASC)
{
	check(ITASC);

	if (!ITASC->IsOwnerActorAuthoritative())
	{
		// 권한이 있어야만(Authoritative) Ability를 주거나 회수할 수 있다.
		return;
	}

	for (const FGameplayAbilitySpecHandle& Handle : AbilitySpecHandles)
	{
		if (Handle.IsValid())
		{
			ITASC->ClearAbility(Handle);
		}
	}

	for (const FActiveGameplayEffectHandle& Handle : GameplayEffectHandles)
	{
		if (Handle.IsValid())
		{
			ITASC->RemoveActiveGameplayEffect(Handle);
		}
	}

	for (UAttributeSet* AttributeSet : GrantedAttributeSets)
	{
		if (AttributeSet)
		{
			ITASC->RemoveSpawnedAttribute(AttributeSet);
		}
	}

	AbilitySpecHandles.Reset();
	GameplayEffectHandles.Reset();
	GrantedAttributeSets.Reset();
}

UITAbilitySet::UITAbilitySet(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}