// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/Fragment/ITItemFragment.h"
#include "ITItemFragment_Equipment.generated.h"

class UGameplayEffect;

UENUM(BlueprintType)
enum class EEquipmentType : uint8
{
	Helmet,
	Armor
};

UCLASS()
class IMITATIONTRIGGER_API UITItemFragment_Equipment : public UITItemFragment
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "IT|Equipment")
	TSubclassOf<UGameplayEffect> EquipmentEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "IT|Equipment", meta = (ClampMin = 1))
	int32 EquipmentTier = 1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "IT|Equipment")
	EEquipmentType EquipmentType = EEquipmentType::Helmet;
};
