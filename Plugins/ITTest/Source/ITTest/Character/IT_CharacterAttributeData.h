// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "AttributeSet.h"
#include "IT_CharacterAttributeData.generated.h"

USTRUCT(BlueprintType)
struct FAttributeInitData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attribute")
	FGameplayAttribute Attribute;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attribute")
	float BaseValue;
};

UCLASS(Blueprintable)
class ITTEST_API UIT_CharacterAttributeData : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attributes")
	TArray<FAttributeInitData> InitAttributes;
};
