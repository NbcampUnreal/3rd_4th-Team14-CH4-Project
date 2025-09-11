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
	AActor*, // EffectInstigator, Effect�� ����Ų ��ü. ĳ���� �Ǵ� ��
	AActor*, // EffectCauser, Effect�� ����Ų ����. �Ѿ�(����ü) �Ǵ� ����
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
