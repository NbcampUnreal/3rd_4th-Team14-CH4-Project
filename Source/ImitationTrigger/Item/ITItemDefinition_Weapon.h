// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/ITItemDefinition.h"
#include "ITItemDefinition_Weapon.generated.h"

class UITAbilitySet;

UCLASS()
class IMITATIONTRIGGER_API UITItemDefinition_Weapon : public UITItemDefinition
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	TObjectPtr<UITAbilitySet> WeaponAbilitySet;
};
