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
}

void UITCombatSet::OnRep_DamageDealt(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, DamageDealt, OldValue);
}

void UITCombatSet::OnRep_KillCount(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, KillCount, OldValue);
}
