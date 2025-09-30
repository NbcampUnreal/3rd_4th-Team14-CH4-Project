// Fill out your copyright notice in the Description page of Project Settings.


#include "ITAmmoSet.h"
#include "Net/UnrealNetwork.h"

UITAmmoSet::UITAmmoSet()
{
	InitAmmo(0.0f);
	InitMaxAmmo(0.0f);
	InitNormalAmmo(0.0f);
	InitSpecialAmmo(0.0f);
}

void UITAmmoSet::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, Ammo, COND_OwnerOnly, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, MaxAmmo, COND_OwnerOnly, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, NormalAmmo, COND_OwnerOnly, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, SpecialAmmo, COND_OwnerOnly, REPNOTIFY_Always);
}

void UITAmmoSet::OnRep_Ammo(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, Ammo, OldValue);
	OnAmmoChanged.Broadcast(GetAmmo(), GetMaxAmmo());
}

void UITAmmoSet::OnRep_MaxAmmo(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, MaxAmmo, OldValue);
	OnAmmoChanged.Broadcast(GetAmmo(), GetMaxAmmo());
}

void UITAmmoSet::OnRep_NormalAmmo(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, NormalAmmo, OldValue);
	OnNormalAmmoChanged.Broadcast(GetNormalAmmo());
}

void UITAmmoSet::OnRep_SpecialAmmo(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, SpecialAmmo, OldValue);
	OnSpecialAmmoChanged.Broadcast(GetSpecialAmmo());
}

void UITAmmoSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	if (Attribute == GetAmmoAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxAmmo());
	}
}
