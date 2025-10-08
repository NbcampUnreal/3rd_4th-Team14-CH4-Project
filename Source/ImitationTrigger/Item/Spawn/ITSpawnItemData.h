// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Item/ITItemActor.h"
#include "ITSpawnItemData.generated.h"

USTRUCT(BlueprintType)
struct FITSpawnItemData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "IT|SpawnItem")
	TSubclassOf<AITItemActor> ItemActorClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "IT|SpawnItem", meta = (ClampMin = "0.0"))
	float Weight = 0.0f;
};
