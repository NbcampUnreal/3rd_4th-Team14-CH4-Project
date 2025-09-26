// Fill out your copyright notice in the Description page of Project Settings.


#include "ITAmmoSet.h"
#include "Net/UnrealNetwork.h"

UITAmmoSet::UITAmmoSet()
{
	InitAmmo(0.0f);
	InitMaxAmmo(0.0f);
}

void UITAmmoSet::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, Ammo, COND_OwnerOnly, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, MaxAmmo, COND_OwnerOnly, REPNOTIFY_Always);
}

void UITAmmoSet::OnRep_Ammo()
{
	OnAmmoChanged.Broadcast(GetAmmo(), GetMaxAmmo());
}

void UITAmmoSet::OnRep_MaxAmmo()
{
	OnAmmoChanged.Broadcast(GetAmmo(), GetMaxAmmo());
}

void UITAmmoSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	if (Attribute == GetAmmoAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxAmmo());
	}
}
