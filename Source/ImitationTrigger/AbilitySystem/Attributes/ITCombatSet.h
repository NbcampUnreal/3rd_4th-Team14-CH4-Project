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

	ATTRIBUTE_ACCESSORS(UITCombatSet, DamageDealt);
	ATTRIBUTE_ACCESSORS(UITCombatSet, KillCount);

	UFUNCTION()
	void OnRep_DamageDealt(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	void OnRep_KillCount(const FGameplayAttributeData& OldValue);

private:
	// 가한 피해량
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_DamageDealt, Category = "Attributes|Combat", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData DamageDealt;

	// 처치 수
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_KillCount, Category = "Attributes|Combat", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData KillCount;
};
