// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Attributes/ITAttributeSet.h"
#include "ITAmmoSet.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnAmmoChanged, float, Ammo, float, MaxAmmo);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnNormalAmmoChanged, float, ReserveAmmo);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSpecialAmmoChanged, float, ReserveAmmo);

UCLASS()
class IMITATIONTRIGGER_API UITAmmoSet : public UITAttributeSet
{
	GENERATED_BODY()

public:
	UITAmmoSet();

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	ATTRIBUTE_ACCESSORS(UITAmmoSet, Ammo);
	ATTRIBUTE_ACCESSORS(UITAmmoSet, MaxAmmo);
	ATTRIBUTE_ACCESSORS(UITAmmoSet, NormalAmmo);
	ATTRIBUTE_ACCESSORS(UITAmmoSet, SpecialAmmo);

	UFUNCTION()
	void OnRep_Ammo(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	void OnRep_MaxAmmo(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	void OnRep_NormalAmmo(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	void OnRep_SpecialAmmo(const FGameplayAttributeData& OldValue);

	FOnAmmoChanged OnAmmoChanged;
	FOnNormalAmmoChanged OnNormalAmmoChanged;
	FOnSpecialAmmoChanged OnSpecialAmmoChanged;

protected:
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;

private:
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Ammo, Category = "Attributes|Ammo",
		Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Ammo;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxAmmo, Category = "Attributes|Ammo",
		Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MaxAmmo;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_NormalAmmo, Category = "Attributes|Ammo",
		Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData NormalAmmo;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_SpecialAmmo, Category = "Attributes|Ammo",
		Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData SpecialAmmo;
};
