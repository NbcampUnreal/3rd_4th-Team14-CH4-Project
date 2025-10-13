// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActiveGameplayEffectHandle.h"
#include "Components/ActorComponent.h"
#include "ITItemManagerComponent.generated.h"

class UITItemDefinition;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCurrentHelmetChanged, int32, CurrentHelmetTier);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCurrentArmorChanged, int32, CurrentArmorTier);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class IMITATIONTRIGGER_API UITItemManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UITItemManagerComponent();
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(BlueprintCallable, Category = "IT|Equipment")
	bool TryAddItem(UITItemDefinition* ItemDefinition);

	// UI 델리게이트
	UPROPERTY(BlueprintAssignable, Category = "IT|Equipment")
	FOnCurrentHelmetChanged OnCurrentHelmetChanged;

	UPROPERTY(BlueprintAssignable, Category = "IT|Equipment")
	FOnCurrentArmorChanged OnCurrentArmorChanged;

protected:
	UFUNCTION()
	void OnRep_CurrentHelmetChanged();

	UFUNCTION()
	void OnRep_CurrentArmorChanged();
	
private:
	bool PutOnEquipment(UITItemDefinition* ItemDefinition);

	void DropEquipment(UITItemDefinition* ItemDefinition);

	// 아이템 티어가 0일 경우, EffectHandle에 접근 금지
	UPROPERTY(ReplicatedUsing = OnRep_CurrentHelmetChanged)
	int32 CurrentHelmetTier = 0;

	UPROPERTY(ReplicatedUsing = OnRep_CurrentArmorChanged)
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
