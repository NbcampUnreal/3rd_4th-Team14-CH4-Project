// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/Fragment/ITItemFragment.h"
#include "ITItemFragment_Potion.generated.h"

class UGameplayAbility;
class UGameplayEffect;

UCLASS(BlueprintType, EditInlineNew, DefaultToInstanced)
class IMITATIONTRIGGER_API UITItemFragment_Potion : public UITItemFragment
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Potion")
	TSubclassOf<UGameplayEffect> PotionEffect;

	// UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Potion")
	// FGameplayAttribute TargetAttribute;
	//
	// UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Potion")
	// float PotionAmount = 0.0f;
	//
	// UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Potion")
	// TSubclassOf<UGameplayAbility> CastingAbility;
	//
	// UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Potion")
	// float CastingTime = 1.0f;

public:
	UITItemFragment_Potion();
};
