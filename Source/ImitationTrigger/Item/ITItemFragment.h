// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "ITItemGameplayTags.h"
#include "ITItemFragment.generated.h"

// 베이스 아이템 프래그먼트
USTRUCT(BlueprintType)
struct FITItemFragment
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Fragment")
	FGameplayTag FragmentTag;
};

// 시각, 청각 효과
USTRUCT(BlueprintType)
struct FITItemFragment_FX : public FITItemFragment
{
	GENERATED_BODY()

	FITItemFragment_FX()
	{
		FragmentTag = ITItemGameplayTags::Item_FX;
	}

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "FX")
	TSoftObjectPtr<class UNiagaraSystem> ItemVFX;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "FX")
	TSoftObjectPtr<class USoundCue> ItemSFX;
};

// 포션
USTRUCT(BlueprintType)
struct FITItemFragment_Potion : public FITItemFragment
{
	GENERATED_BODY()

	FITItemFragment_Potion()
	{
		FragmentTag = ITItemGameplayTags::Item_Use_Potion;
	}

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Potion")
	TSubclassOf<class UGameplayEffect> PotionEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Potion")
	float PotionAmount = 0.0f;
};

// 수류탄
USTRUCT(BlueprintType)
struct FITItemFragment_Grenade : public FITItemFragment
{
	GENERATED_BODY()

	FITItemFragment_Grenade()
	{
		FragmentTag = ITItemGameplayTags::Item_Use_Grenade;
	}

	// 투척 어빌리티
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Grenade")
	TSubclassOf<class UGameplayAbility> ProjectileAbility;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Grenade")
	float ExplosionDelay = 1.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Grenade")
	float ExplosionRadius = 1000.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Grenade")
	TSubclassOf<class UGameplayEffect> GrenadeEffect;
};
