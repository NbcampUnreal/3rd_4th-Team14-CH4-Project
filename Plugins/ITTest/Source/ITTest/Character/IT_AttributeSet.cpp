// Fill out your copyright notice in the Description page of Project Settings.


#include "IT_AttributeSet.h"

UIT_AttributeSet::UIT_AttributeSet()
{
}

void UIT_AttributeSet::PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const
{
	Super::PreAttributeBaseChange(Attribute, NewValue);
}

void UIT_AttributeSet::PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);
}
