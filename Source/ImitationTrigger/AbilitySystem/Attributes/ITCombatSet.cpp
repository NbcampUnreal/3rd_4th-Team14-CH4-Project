#include "AbilitySystem/Attributes/ITCombatSet.h"
#include "Net/UnrealNetwork.h"


UITCombatSet::UITCombatSet()
{
	InitDamageDealt(0.0f);
	InitKillCount(0.0f);
}

void UITCombatSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, DamageDealt, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, KillCount, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, UltimateGauge, COND_None, REPNOTIFY_Always);
}

void UITCombatSet::OnRep_DamageDealt(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, DamageDealt, OldValue);
}

void UITCombatSet::OnRep_KillCount(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, KillCount, OldValue);
}

void UITCombatSet::OnRep_UltimateGauge(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, UltimateGauge, OldValue);
}

void UITCombatSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	if (Attribute == GetUltimateGaugeAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, 100.0f);
	}
}

void UITCombatSet::PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const
{
	Super::PreAttributeBaseChange(Attribute, NewValue);

	if (Attribute == GetUltimateGaugeAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, 100.0f);
	}
}
