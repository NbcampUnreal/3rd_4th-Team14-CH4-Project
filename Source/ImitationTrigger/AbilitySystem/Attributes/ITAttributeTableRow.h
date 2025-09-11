#pragma once

#include "AttributeSet.h"
#include "ITAttributeTableRow.generated.h"

USTRUCT(BlueprintType)
struct FITAttributeTableRow : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attribute")
	FGameplayAttribute Attribute;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attribute")
	float InitValue = 0.0f;
};