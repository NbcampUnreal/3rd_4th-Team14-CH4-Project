#pragma once

#include "AbilitySystem/Attributes/ITAttributeSet.h"
#include "ITCombatSet.generated.h"

UCLASS()
class IMITATIONTRIGGER_API UITCombatSet : public UITAttributeSet
{
	GENERATED_BODY()
	
public:
	UITCombatSet();
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;

	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const override;

	ATTRIBUTE_ACCESSORS(UITCombatSet, DamageDealt);
	ATTRIBUTE_ACCESSORS(UITCombatSet, KillCount);
	ATTRIBUTE_ACCESSORS(UITCombatSet, UltimateGauge);

	UFUNCTION()
	void OnRep_DamageDealt(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	void OnRep_KillCount(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	void OnRep_UltimateGauge(const FGameplayAttributeData& OldValue);

private:
	// 가한 피해량
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_DamageDealt, Category = "Attributes|Combat", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData DamageDealt;

	// 처치 수
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_KillCount, Category = "Attributes|Combat", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData KillCount;

	// 처치 수
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_UltimateGauge, Category = "Attributes|Combat", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData UltimateGauge;
};
