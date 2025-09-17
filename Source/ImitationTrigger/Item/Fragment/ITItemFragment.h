// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UObject/Object.h"
#include "ITItemFragment.generated.h"

class UITItemInstance;

UCLASS(Abstract, EditInlineNew, DefaultToInstanced)
class IMITATIONTRIGGER_API UITItemFragment : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Fragment")
	FGameplayTag FragmentTag;
};
