// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActiveGameplayEffectHandle.h"
#include "Components/ActorComponent.h"
#include "ITItemManagerComponent.generated.h"

class UITItemDefinition;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class IMITATIONTRIGGER_API UITItemManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UITItemManagerComponent();
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(BlueprintCallable, Category = "IT|Item")
	bool TryAddItem(UITItemDefinition* ItemDefinition);

private:
	bool PutOnEquipment(UITItemDefinition* ItemDefinition);

	void DropEquipment(UITItemDefinition* ItemDefinition);

	// 아이템 티어가 0일 경우, EffectHandle에 접근 금지
	UPROPERTY(Replicated)
	int32 CurrentHelmetTier = 0;

	UPROPERTY(Replicated)
	int32 CurrentArmorTier = 0;

	UPROPERTY()
	FActiveGameplayEffectHandle HelmetEffectHandle;

	UPROPERTY()
	FActiveGameplayEffectHandle ArmorEffectHandle;

	UPROPERTY()
	TObjectPtr<UITItemDefinition> HelmetDefinition;
	
	UPROPERTY()
	TObjectPtr<UITItemDefinition> ArmorDefinition;
};
