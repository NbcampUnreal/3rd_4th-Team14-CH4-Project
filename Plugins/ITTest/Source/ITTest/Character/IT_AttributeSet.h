// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "IT_AttributeSet.generated.h"

// public Getter, Setter 자동 생성 매크로
#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

UCLASS()
class ITTEST_API UIT_AttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	UIT_AttributeSet();

	// Attribute의 BaseValue가 변경되기 전 호출되는 함수 / Clamp, 레벨 업 로직 등
	virtual void PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const override;
	// Attribute가 Effect에 의해 변경된 후 호출되는 함수
	virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data) override;

public:
	// Attribute 속성 구현
	UPROPERTY(BlueprintReadOnly, Category = "Attributes")
	FGameplayAttributeData WalkSpeed;
	ATTRIBUTE_ACCESSORS(UIT_AttributeSet, WalkSpeed);

	UPROPERTY(BlueprintReadOnly, Category = "Attributes")
	FGameplayAttributeData SprintSpeed;
	ATTRIBUTE_ACCESSORS(UIT_AttributeSet, SprintSpeed);

	UPROPERTY(BlueprintReadOnly, Category = "Attributes")
	FGameplayAttributeData JumpSpeed;
	ATTRIBUTE_ACCESSORS(UIT_AttributeSet, JumpSpeed);
};
