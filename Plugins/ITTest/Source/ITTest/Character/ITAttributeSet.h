// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "ITAttributeSet.generated.h"

// public Getter, Setter 자동 생성 매크로
#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

UCLASS()
class ITTEST_API UITAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	UITAttributeSet();

	/**
	* @brief Attribute의 CurrentValue가 변경되기 전 호출되는 함수 
	* @details 값을 검증 또는 조정(Clamp) / Ex. 회복, 피격 등
	*/
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	/**
	* @brief Attribute의 BaseValue가 변경되기 전 호출되는 함수 
	* @details 기본 스탯에 대한 하드캡(HardCap) / Ex. 스탯 상승, 템 장착 등
	*/
	virtual void PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const override;

	/**
	* @brief Attribute의 CurrentValue가 변경된 후 호출되는 함수 
	* @details 값 검증 및 로직 실행 / Ex. 경험치 체크 및 레벨 증가 등
	*/
	virtual void PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) override;
	/**
	* @brief Attribute의 BaseValue가 변경된 후 호출되는 함수 
	* @details 값 검증 및 업데이트 / Ex. UI 정보 업데이트 등
	*/
	virtual void PostAttributeBaseChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) const override;

	/**
	 * @brief Attribute가 Effect에 의해 변경된 후 호출되는 함수
	 * @details Effect의 데이터가 필요한 로직 수행 / Ex. 킬로그, 상태이상 등
	 */
	virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data) override;

public:
	// Attribute 속성 구현
	UPROPERTY(BlueprintReadOnly, Category = "Attributes")
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UITAttributeSet, Health);

	UPROPERTY(BlueprintReadOnly, Category = "Attributes")
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UITAttributeSet, MaxHealth);

	UPROPERTY(BlueprintReadOnly, Category = "Attributes")
	FGameplayAttributeData Shield;
	ATTRIBUTE_ACCESSORS(UITAttributeSet, Shield);

	UPROPERTY(BlueprintReadOnly, Category = "Attributes")
	FGameplayAttributeData MaxShield;
	ATTRIBUTE_ACCESSORS(UITAttributeSet, MaxShield);

	UPROPERTY(BlueprintReadOnly, Category = "Attributes")
	FGameplayAttributeData Stamina;
	ATTRIBUTE_ACCESSORS(UITAttributeSet, Stamina);

	UPROPERTY(BlueprintReadOnly, Category = "Attributes")
	FGameplayAttributeData MaxStamina;
	ATTRIBUTE_ACCESSORS(UITAttributeSet, MaxStamina);

	UPROPERTY(BlueprintReadOnly, Category = "Attributes")
	FGameplayAttributeData MeleeDamage;
	ATTRIBUTE_ACCESSORS(UITAttributeSet, MeleeDamage);

	UPROPERTY(BlueprintReadOnly, Category = "Attributes")
	FGameplayAttributeData WeaponDamage;
	ATTRIBUTE_ACCESSORS(UITAttributeSet, WeaponDamage);

	UPROPERTY(BlueprintReadOnly, Category = "Attributes")
	FGameplayAttributeData Defense;
	ATTRIBUTE_ACCESSORS(UITAttributeSet, Defense);

	UPROPERTY(BlueprintReadOnly, Category = "Attributes")
	FGameplayAttributeData WalkSpeed;
	ATTRIBUTE_ACCESSORS(UITAttributeSet, WalkSpeed);

	UPROPERTY(BlueprintReadOnly, Category = "Attributes")
	FGameplayAttributeData SprintSpeed;
	ATTRIBUTE_ACCESSORS(UITAttributeSet, SprintSpeed);

	UPROPERTY(BlueprintReadOnly, Category = "Attributes")
	FGameplayAttributeData JumpSpeed;
	ATTRIBUTE_ACCESSORS(UITAttributeSet, JumpSpeed);
};
