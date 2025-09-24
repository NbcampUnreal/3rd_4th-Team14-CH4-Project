// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "ITWeaponFireAbility.generated.h"

class UITItemDefinition_Weapon;
class UGameplayEffect;

UCLASS(Blueprintable, Abstract)
class IMITATIONTRIGGER_API UITWeaponFireAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UITWeaponFireAbility();

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	                             const FGameplayAbilityActorInfo* ActorInfo,
	                             const FGameplayAbilityActivationInfo ActivationInfo,
	                             const FGameplayEventData* TriggerEventData) override;

	virtual void ApplyCooldown(const FGameplayAbilitySpecHandle Handle,
	                           const FGameplayAbilityActorInfo* ActorInfo,
	                           const FGameplayAbilityActivationInfo ActivationInfo) const override;

	// 상속 후, 발사 방식 구현 // Ex. 히트스캔(라인트레이스), 투사체, 범위 공격 등
	UFUNCTION(BlueprintCallable, Category = "IT|Ability")
	virtual void Fire(const FVector& StartLocation, const FVector& FireDirection);

	// TODO: 팀 태그 추가 시, 피아 식별 로직 추가
	UFUNCTION(BlueprintCallable, Category = "IT|Ability")
	void ApplyWeaponDamage(AActor* TargetActor);

	UFUNCTION(BlueprintCallable, Category = "IT|Ability")
	UITItemDefinition_Weapon* GetWeaponDefinition() const;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "IT|Effect")
	TSubclassOf<UGameplayEffect> DamageEffect;
};
