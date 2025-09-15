#include "AbilitySystem/Attributes/ITAttributeSet.h"
#include "AbilitySystem/ITAbilitySystemComponent.h"

UITAttributeSet::UITAttributeSet()
{
}

UITAbilitySystemComponent* UITAttributeSet::GetITAbilitySystemComponent() const
{
	return Cast<UITAbilitySystemComponent>(GetOwningAbilitySystemComponent());
}
