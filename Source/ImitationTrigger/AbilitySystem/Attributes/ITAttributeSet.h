#pragma once

#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "ITAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

DECLARE_MULTICAST_DELEGATE_SixParams(FITAttributeEvent,
	AActor*, // EffectInstigator, Effect를 일으킨 주체. 캐릭터 또는 폰
	AActor*, // EffectCauser, Effect를 일으킨 원인. 총알(투사체) 또는 무기
	const FGameplayEffectSpec*, // EffectSpec
	float, // EffectMagnitude
	float, // OldValue
	float); // NewValue


class UITAbilitySystemComponent;

UCLASS()
class IMITATIONTRIGGER_API UITAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	UITAttributeSet();
	UITAbilitySystemComponent* GetITAbilitySystemComponent() const;
};
