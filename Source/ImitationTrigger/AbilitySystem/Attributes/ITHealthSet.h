#pragma once

#include "AbilitySystem/Attributes/ITAttributeSet.h"
#include "ITHealthSet.generated.h"

struct FGameplayEffectSpec;


UCLASS()
class IMITATIONTRIGGER_API UITHealthSet : public UITAttributeSet
{
	GENERATED_BODY()

public:
	UITHealthSet();
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;

	ATTRIBUTE_ACCESSORS(UITHealthSet, Health);
	ATTRIBUTE_ACCESSORS(UITHealthSet, MaxHealth);
	ATTRIBUTE_ACCESSORS(UITHealthSet, Shield);
	ATTRIBUTE_ACCESSORS(UITHealthSet, MaxShield);
	ATTRIBUTE_ACCESSORS(UITHealthSet, HeadshotResistances);
	ATTRIBUTE_ACCESSORS(UITHealthSet, GainHealth);
	ATTRIBUTE_ACCESSORS(UITHealthSet, GainShield);
	ATTRIBUTE_ACCESSORS(UITHealthSet, GainDamage);
	ATTRIBUTE_ACCESSORS(UITHealthSet, GainHeadshotDamage);

	UFUNCTION()
	void OnRep_Health(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	void OnRep_MaxHealth(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	void OnRep_Shield(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	void OnRep_MaxShield(const FGameplayAttributeData& OldValue);

	FITAttributeEvent OnHealthChanged;
	FITAttributeEvent OnMaxHealthChanged;
	FITAttributeEvent OnShieldChanged;
	FITAttributeEvent OnMaxShieldChanged;
	FITAttributeEvent OnOutOfHealth;
	FITAttributeEvent OnOutOfShield;

protected:
	virtual bool PreGameplayEffectExecute(FGameplayEffectModCallbackData& Data) override;
	virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data) override;
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const override;
	virtual void PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) override;
	virtual void PostAttributeBaseChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) const override;

	void ClampAttribute(const FGameplayAttribute& Attribute, float& NewValue) const;

private:
	float CalculateHeadshotDamage(float InDamage);
	void ApplyDamage(float InDamage);

	// 현재 체력. 이 값이 0이 되면 캐릭터가 쓰러진다.
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Health, Category = "Attributes|Health", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Health;

	// 최대 체력.
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxHealth, Category = "Attributes|Health", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MaxHealth;

	// 현재 보호막(실드). 공격을 받을 경우, 보호막이 먼저 줄어든다. 보호막이 0이 되면, 그 때 체력이 줄어든다.
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Shield, Category = "Attributes|Health", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Shield;

	// 최대 보호막. 최대 보호막 양은 장비에 따라 바뀔 수 있다.
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxShield, Category = "Attributes|Health", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MaxShield;

	// 헤드샷 피해량 저항. 이 값은 헬멧에 의해 바뀔 수 있다.
	UPROPERTY(BlueprintReadOnly, Category = "Attributes|Health", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData HeadshotResistances;

	// Meta Attribute, 받은 체력 회복량
	UPROPERTY(BlueprintReadOnly, Category = "Attributes|Health", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData GainHealth;

	// Meta Attribute, 받은 보호막 회복량
	UPROPERTY(BlueprintReadOnly, Category = "Attributes|Health", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData GainShield;

	// Meta Attribute, 받은 일반 데미지
	UPROPERTY(BlueprintReadOnly, Category = "Attributes|Health", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData GainDamage;

	// Meta Attribute, 받은 헤드샷 데미지
	UPROPERTY(BlueprintReadOnly, Category = "Attributes|Health", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData GainHeadshotDamage;

	// 값이 변경되었는지 확인하기 위한 Before 변수. 값이 변경되었다면, FITAttributeEvent Delegate를 호출한다.
	float BeforeHealth;
	float BeforeMaxHealth;
	float BeforeShield;
	float BeforeMaxShield;
};
