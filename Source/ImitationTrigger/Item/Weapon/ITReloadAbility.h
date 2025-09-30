// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "ITReloadAbility.generated.h"

UCLASS()
class IMITATIONTRIGGER_API UITReloadAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UITReloadAbility();

protected:
	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle,
	                                const FGameplayAbilityActorInfo* ActorInfo,
	                                const FGameplayTagContainer* SourceTags = nullptr,
	                                const FGameplayTagContainer* TargetTags = nullptr,
	                                FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	                             const FGameplayAbilityActorInfo* ActorInfo,
	                             const FGameplayAbilityActivationInfo ActivationInfo,
	                             const FGameplayEventData* TriggerEventData) override;

	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle,
	                        const FGameplayAbilityActorInfo* ActorInfo,
	                        const FGameplayAbilityActivationInfo ActivationInfo,
	                        bool bReplicateEndAbility,
	                        bool bWasCancelled) override;

	UFUNCTION()
	void OnMontageCompleted();

	UFUNCTION()
	void OnMontageCancelled();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "IT|Reload")
	TSubclassOf<UGameplayEffect> ReloadEffectClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "IT|Reload")
	TSubclassOf<UGameplayEffect> ConsumeReserveAmmoEffectClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "IT|Reload")
	TObjectPtr<UAnimMontage> ReloadMontage;
};
