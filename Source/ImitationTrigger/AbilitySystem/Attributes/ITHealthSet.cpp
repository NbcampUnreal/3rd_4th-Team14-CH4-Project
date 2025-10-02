#include "AbilitySystem/Attributes/ITHealthSet.h"
#include "Net/UnrealNetwork.h"
#include "GameplayEffectExtension.h"

UITHealthSet::UITHealthSet()
{
	InitHealth(0.0f);
	InitMaxHealth(0.0f);
	InitShield(0.0f);
	InitMaxShield(0.0f);
	InitDamageResistances(0.0f);
	InitHeadshotResistances(0.0f);
	InitGainHealth(0.0f);
	InitGainShield(0.0f);
	InitGainDamage(0.0f);
	InitGainHeadshotDamage(0.0f);
}

void UITHealthSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, MaxHealth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, Shield, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, MaxShield, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, DamageResistances, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, HeadshotResistances, COND_None, REPNOTIFY_Always);
}

bool UITHealthSet::PreGameplayEffectExecute(FGameplayEffectModCallbackData& Data)
{
	if (!Super::PreGameplayEffectExecute(Data))
	{
		return false;
	}

	BeforeHealth = GetHealth();
	BeforeMaxHealth = GetMaxHealth();
	BeforeShield = GetShield();
	BeforeMaxShield = GetMaxShield();

	return true;
}

void UITHealthSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	const FGameplayEffectContextHandle& EffectContext = Data.EffectSpec.GetEffectContext();
	AActor* Instigator = EffectContext.GetOriginalInstigator();
	AActor* Causer = EffectContext.GetEffectCauser();

	if (Data.EvaluatedData.Attribute == GetGainDamageAttribute())
	{
		const float DamageValue = CalculateNormalDamage(GetGainDamage());
		ApplyDamage(DamageValue);
		SetGainDamage(0.0f);
	}
	else if (Data.EvaluatedData.Attribute == GetGainHeadshotDamageAttribute())
	{
		const float HeadDamageValue = GetGainHeadshotDamage();
		const float DamageValue = CalculateHeadshotDamage(HeadDamageValue);
		ApplyDamage(DamageValue);
		SetGainHeadshotDamage(0.0f);
	}
	else if (Data.EvaluatedData.Attribute == GetGainHealthAttribute())
	{
		const float RestoreValue = GetGainHealth();
		const float NewValue = FMath::Clamp(GetHealth() + RestoreValue, 0, GetMaxHealth());
		SetHealth(NewValue);
		SetGainHealth(0.0f);
	}
	else if (Data.EvaluatedData.Attribute == GetGainShieldAttribute())
	{
		const float RestoreValue = GetGainShield();
		const float NewValue = FMath::Clamp(GetShield() + RestoreValue, 0, GetMaxShield());
		SetShield(NewValue);
		SetGainShield(0.0f);
	}

	if (GetHealth() != BeforeHealth)
	{
		OnHealthChanged.Broadcast(Instigator, Causer, &(Data.EffectSpec), Data.EvaluatedData.Magnitude, BeforeHealth, GetHealth());
		
		if (GetHealth() <= 0)
		{
			OnOutOfHealth.Broadcast(Instigator, Causer, &(Data.EffectSpec), Data.EvaluatedData.Magnitude, BeforeHealth, GetHealth());
		}
	}
	if (GetMaxHealth() != BeforeMaxHealth)
	{
		OnMaxHealthChanged.Broadcast(Instigator, Causer, &(Data.EffectSpec), Data.EvaluatedData.Magnitude, BeforeMaxHealth, GetMaxHealth());
	}
	if (GetShield() != BeforeShield)
	{
		OnShieldChanged.Broadcast(Instigator, Causer, &(Data.EffectSpec), Data.EvaluatedData.Magnitude, BeforeShield, GetShield());
		
		if (GetShield() <= 0)
		{
			OnOutOfShield.Broadcast(Instigator, Causer, &(Data.EffectSpec), Data.EvaluatedData.Magnitude, BeforeShield, GetShield());
		}
	}
	if (GetMaxShield() != BeforeMaxShield)
	{
		OnMaxShieldChanged.Broadcast(Instigator, Causer, &(Data.EffectSpec), Data.EvaluatedData.Magnitude, BeforeMaxShield, GetMaxShield());
	}
}

void UITHealthSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	ClampAttribute(Attribute, NewValue);
}

void UITHealthSet::PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const
{
	Super::PreAttributeBaseChange(Attribute, NewValue);

	ClampAttribute(Attribute, NewValue);
}

void UITHealthSet::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
{
	Super::PostAttributeChange(Attribute, OldValue, NewValue);
}

void UITHealthSet::PostAttributeBaseChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) const
{
	Super::PostAttributeBaseChange(Attribute, OldValue, NewValue);
}

void UITHealthSet::ClampAttribute(const FGameplayAttribute& Attribute, float& NewValue) const
{
	if (Attribute == GetHealthAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0, GetMaxHealth());
	}
	else if (Attribute == GetShieldAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0, GetMaxShield());
	}
}

float UITHealthSet::CalculateNormalDamage(float InDamage)
{
	const float ReducedRate = FMath::Max(0.0f, 1.0 - GetDamageResistances());
	return InDamage * ReducedRate;
}

float UITHealthSet::CalculateHeadshotDamage(float InDamage)
{
	const float ReducedRate = FMath::Max(0.0f, 1.0 - GetHeadshotResistances());
	return InDamage * ReducedRate;
}

void UITHealthSet::ApplyDamage(float InDamage)
{
	float RemainDamage = InDamage;

	const float ReducedShield = FMath::Min(GetShield(), RemainDamage);
	SetShield(GetShield() - ReducedShield);
	RemainDamage -= ReducedShield;
	
	if (!FMath::IsNearlyZero(RemainDamage))
	{
		const float ReducedHealth = FMath::Min(GetHealth(), RemainDamage);
		SetHealth(GetHealth() - ReducedHealth);
	}
}

void UITHealthSet::OnRep_Health(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, Health, OldValue);

	float OldFloatValue = OldValue.GetCurrentValue();
	float NewFloatValue = GetHealth();
	float Magnitude = NewFloatValue - OldFloatValue;
	OnHealthChanged.Broadcast(nullptr, nullptr, nullptr, Magnitude, OldFloatValue, NewFloatValue);

	if (GetHealth() <= 0)
	{
		OnOutOfHealth.Broadcast(nullptr, nullptr, nullptr, Magnitude, OldFloatValue, NewFloatValue);
	}
}

void UITHealthSet::OnRep_MaxHealth(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, MaxHealth, OldValue);

	float OldFloatValue = OldValue.GetCurrentValue();
	float NewFloatValue = GetMaxHealth();
	float Magnitude = NewFloatValue - OldFloatValue;
	OnMaxHealthChanged.Broadcast(nullptr, nullptr, nullptr, Magnitude, OldFloatValue, NewFloatValue);
}

void UITHealthSet::OnRep_Shield(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, Shield, OldValue);

	float OldFloatValue = OldValue.GetCurrentValue();
	float NewFloatValue = GetShield();
	float Magnitude = NewFloatValue - OldFloatValue;
	OnShieldChanged.Broadcast(nullptr, nullptr, nullptr, Magnitude, OldFloatValue, NewFloatValue);

	if (GetShield() <= 0)
	{
		OnOutOfShield.Broadcast(nullptr, nullptr, nullptr, Magnitude, OldFloatValue, NewFloatValue);
	}
}

void UITHealthSet::OnRep_MaxShield(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, MaxShield, OldValue);

	float OldFloatValue = OldValue.GetCurrentValue();
	float NewFloatValue = GetMaxShield();
	float Magnitude = NewFloatValue - OldFloatValue;
	OnMaxShieldChanged.Broadcast(nullptr, nullptr, nullptr, Magnitude, OldFloatValue, NewFloatValue);
}
