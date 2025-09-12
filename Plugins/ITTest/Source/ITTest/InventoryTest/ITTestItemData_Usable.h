// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ITTestItemData.h"
#include "ITTestItemData_Usable.generated.h"

class UGameplayAbility;

UCLASS()
class ITTEST_API UITTestItemData_Usable : public UITTestItemData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item|Usable")
	TSubclassOf<UGameplayAbility> ItemAbility;
};
