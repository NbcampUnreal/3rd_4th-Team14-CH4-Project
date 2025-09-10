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

void UITAbilitySet::GiveToAbilitySystem(UITAbilitySystemComponent* ITASC, FITAbilitySet_GrantedHandles* OutGrantedHandles, UObject* SourceObject) const
{
	check(ITASC);

	if (!ITASC->IsOwnerActorAuthoritative())
	{
		// 권한이 있어야만(Authoritative) Ability를 주거나 회수할 수 있다.
		return;
	}

	GrantAttributeSets(ITASC, OutGrantedHandles);
	GrantAbilities(ITASC, OutGrantedHandles, SourceObject);
	GrantGameplayEffects(ITASC, OutGrantedHandles);
}

void UITAbilitySet::GrantAttributeSets(UITAbilitySystemComponent* ITASC, FITAbilitySet_GrantedHandles* OutGrantedHandles) const
{
	for (int32 SetIndex = 0; SetIndex < GrantedAttributes.Num(); SetIndex++)
	{
		const FITAbilitySet_AttributeSet& SetToGrant = GrantedAttributes[SetIndex];

		if (!IsValid(SetToGrant.AttributeSet))
		{
			UE_LOG(LogTemp, Error, TEXT("GrantedAttributes[%d] on ability set [%s] is not valid"), SetIndex, *GetNameSafe(this));
			continue;
		}

		AActor* ASCOwner = ITASC->GetOwner();
		UAttributeSet* NewSet = NewObject<UAttributeSet>(ASCOwner, SetToGrant.AttributeSet);
		ITASC->AddAttributeSetSubobject(NewSet);

		if (OutGrantedHandles)
		{
			OutGrantedHandles->AddAttributeSet(NewSet);
		}
	}
}

void UITAbilitySet::GrantAbilities(UITAbilitySystemComponent* ITASC, FITAbilitySet_GrantedHandles* OutGrantedHandles, UObject* SourceObject) const
{
	for (int32 AbilityIndex = 0; AbilityIndex < GrantedGameplayAbilities.Num(); AbilityIndex++)
	{
		const FITAbilitySet_GameplayAbility& AbilityToGrant = GrantedGameplayAbilities[AbilityIndex];

		if (!IsValid(AbilityToGrant.Ability))
		{
			UE_LOG(LogTemp, Error, TEXT("GrantedGameplayAbilities[%d] on ability set [%s] is not valid."), AbilityIndex, *GetNameSafe(this));
			continue;
		}

		UGameplayAbility* AbilityCDO = AbilityToGrant.Ability->GetDefaultObject<UGameplayAbility>();

		FGameplayAbilitySpec AbilitySpec(AbilityCDO, AbilityToGrant.AbilityLevel);
		AbilitySpec.SourceObject = SourceObject;
		AbilitySpec.GetDynamicSpecSourceTags().AddTag(AbilityToGrant.InputTag);

		const FGameplayAbilitySpecHandle AbilitySpecHandle = ITASC->GiveAbility(AbilitySpec);

		if (OutGrantedHandles)
		{
			OutGrantedHandles->AddAbilitySpecHandle(AbilitySpecHandle);
		}
	}
}

void UITAbilitySet::GrantGameplayEffects(UITAbilitySystemComponent* ITASC, FITAbilitySet_GrantedHandles* OutGrantedHandles) const
{
	for (int32 EffectIndex = 0; EffectIndex < GrantedGameplayEffects.Num(); ++EffectIndex)
	{
		const FITAbilitySet_GameplayEffect& EffectToGrant = GrantedGameplayEffects[EffectIndex];

		if (!IsValid(EffectToGrant.GameplayEffect))
		{
			UE_LOG(LogTemp, Error, TEXT("GrantedGameplayEffects[%d] on ability set [%s] is not valid"), EffectIndex, *GetNameSafe(this));
			continue;
		}

		const UGameplayEffect* GameplayEffect = EffectToGrant.GameplayEffect->GetDefaultObject<UGameplayEffect>();
		const FGameplayEffectContextHandle ContextHandle = ITASC->MakeEffectContext();
		const FActiveGameplayEffectHandle GameplayEffectHandle = ITASC->ApplyGameplayEffectToSelf(GameplayEffect, EffectToGrant.EffectLevel, ContextHandle);

		if (OutGrantedHandles)
		{
			OutGrantedHandles->AddGameplayEffectHandle(GameplayEffectHandle);
		}
	}
}
