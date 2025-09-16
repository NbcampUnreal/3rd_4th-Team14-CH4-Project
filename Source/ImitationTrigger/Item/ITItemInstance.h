// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ITItemInstance.generated.h"

class UITItemDefinition;

UCLASS(BlueprintType)
class IMITATIONTRIGGER_API UITItemInstance : public UObject
{
	GENERATED_BODY()

public:
	virtual bool IsSupportedForNetworking() const override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	void InitItemInstance(UITItemDefinition* InItemDefinition, int32 InitQuantity = 1);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Replicated, Category = "Item")
	TObjectPtr<UITItemDefinition> ItemDefinition;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Replicated, Category = "Item")
	int32 ItemQuantity = 0;
};
