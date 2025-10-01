// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ITPawnDataList.generated.h"

class UITPawnData;
/**
 * 
 */
UCLASS()
class IMITATIONTRIGGER_API UITPawnDataList : public UDataAsset
{
	GENERATED_BODY()

public:
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Pawn Data")
	TArray<TObjectPtr<UITPawnData>> PawnDatas;
	
};
