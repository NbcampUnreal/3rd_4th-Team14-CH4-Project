// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/Weapon/ITWeaponFireAbility.h"
#include "ITWeaponFireAbility_Hitscan.generated.h"

UCLASS()
class IMITATIONTRIGGER_API UITWeaponFireAbility_Hitscan : public UITWeaponFireAbility
{
	GENERATED_BODY()

protected:
	virtual void Fire(const FVector& TraceStart, const FVector& TraceDirection) override;
};
