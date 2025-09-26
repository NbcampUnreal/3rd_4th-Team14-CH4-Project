// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Attributes/ITAttributeSet.h"
#include "ITAmmoSet.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnAmmoChanged, float, Ammo, float, MaxAmmo);

UCLASS()
class IMITATIONTRIGGER_API UITAmmoSet : public UITAttributeSet
{
	GENERATED_BODY()

public:
	UITAmmoSet();

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	ATTRIBUTE_ACCESSORS(UITAmmoSet, Ammo);
	ATTRIBUTE_ACCESSORS(UITAmmoSet, MaxAmmo);

	UFUNCTION()
	void OnRep_Ammo();

	UFUNCTION()
	void OnRep_MaxAmmo();

	FOnAmmoChanged OnAmmoChanged;

protected:
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;

private:
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Ammo, Category = "Attributes|Ammo", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Ammo;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxAmmo, Category = "Attributes|Ammo", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MaxAmmo;
};
