#pragma once

#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "ITInputConfig.generated.h"

class UInputAction;
class UInputMappingContext;

USTRUCT(BlueprintType)
struct FInputMappingContextAndPriority
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputMappingContext> InputMapping;

	UPROPERTY(EditAnywhere, Category = "Input")
	int32 Priority = 0;
};


// Pair of (Gameplay Tag, Input Action)
USTRUCT(BlueprintType)
struct FITInputAction
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<const UInputAction> InputAction = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Meta = (Categories = "InputTag"))
	FGameplayTag InputTag;
};


UCLASS(BlueprintType, Const)
class IMITATIONTRIGGER_API UITInputConfig : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UITInputConfig(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintCallable, Category = "Lyra|Pawn")
	const UInputAction* FindNativeInputActionForTag(const FGameplayTag& InputTag, bool bLogNotFound = true) const;

	UFUNCTION(BlueprintCallable, Category = "Lyra|Pawn")
	const UInputAction* FindAbilityInputActionForTag(const FGameplayTag& InputTag, bool bLogNotFound = true) const;

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Meta = (TitleProperty = "InputAction"))
	TArray<FITInputAction> NativeInputActions;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Meta = (TitleProperty = "InputAction"))
	TArray<FITInputAction> AbilityInputActions;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FInputMappingContextAndPriority> DefaultMappings;
};
