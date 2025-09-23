// Fill out your copyright notice in the Description page of Project Settings.


#include "ITWeaponFireAbility.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "ITItemDefinition_Weapon.h"
#include "AI/NavigationSystemBase.h"
#include "Item/ITItemGameplayTags.h"
#include "Item/ITItemInstance.h"

UITWeaponFireAbility::UITWeaponFireAbility()
{
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::ServerInitiated;
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UITWeaponFireAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                           const FGameplayAbilityActorInfo* ActorInfo,
                                           const FGameplayAbilityActivationInfo ActivationInfo,
                                           const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	AActor* AvatarActor = GetAvatarActorFromActorInfo();
	if (!AvatarActor || !AvatarActor->HasAuthority() || !CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	UITItemInstance* WeaponInstance = Cast<UITItemInstance>(GetSourceObject(Handle, ActorInfo));
	if (!WeaponInstance)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	UITItemDefinition_Weapon* WeaponDefinition = Cast<UITItemDefinition_Weapon>(WeaponInstance->GetItemDefinition());
	if (!WeaponDefinition)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	if (CooldownGameplayEffectClass)
	{
		float CooldownDuration = 60.0f / WeaponDefinition->FireRate;
		FGameplayEffectSpecHandle CooldownSpecHandle = MakeOutgoingGameplayEffectSpec(CooldownGameplayEffectClass);
		if (CooldownSpecHandle.IsValid())
		{
			CooldownSpecHandle.Data->SetSetByCallerMagnitude(ITItemGameplayTags::Weapon_Cooldown, CooldownDuration);
			if (UAbilitySystemComponent* SourceASC = GetAbilitySystemComponentFromActorInfo())
			{
				SourceASC->ApplyGameplayEffectSpecToSelf(*CooldownSpecHandle.Data.Get());
			}
		}
	}

	FVector TraceStart;
	FRotator ViewRotation;
	ActorInfo->PlayerController->GetPlayerViewPoint(TraceStart, ViewRotation);
	FVector TraceDirection = ViewRotation.Vector();

	Fire(TraceStart, TraceDirection);

	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}

void UITWeaponFireAbility::ApplyCooldown(const FGameplayAbilitySpecHandle Handle,
                                         const FGameplayAbilityActorInfo* ActorInfo,
                                         const FGameplayAbilityActivationInfo ActivationInfo) const
{
	return;
}

void UITWeaponFireAbility::Fire(const FVector& StartLocation, const FVector& FireDirection)
{
}

void UITWeaponFireAbility::ApplyWeaponDamage(AActor* TargetActor)
{
	if (!TargetActor || !DamageEffect)
	{
		return;
	}

	FGameplayAbilitySpec* Spec = GetCurrentAbilitySpec();
	if (!Spec)
	{
		return;
	}
	const FGameplayAbilityActorInfo* ActorInfo = GetCurrentActorInfo();
	if (!ActorInfo)
	{
		return;
	}


	UITItemInstance* WeaponInstance = Cast<UITItemInstance>(Spec->SourceObject.Get());
	if (!WeaponInstance)
	{
		return;
	}

	UITItemDefinition_Weapon* WeaponDefinition = Cast<UITItemDefinition_Weapon>(WeaponInstance->GetItemDefinition());
	if (!WeaponDefinition)
	{
		return;
	}

	float Damage = WeaponDefinition->Damage;
	FGameplayEffectSpecHandle DamageSpecHandle = MakeOutgoingGameplayEffectSpec(DamageEffect);
	if (DamageSpecHandle.IsValid())
	{
		DamageSpecHandle.Data->SetSetByCallerMagnitude(ITItemGameplayTags::Weapon_Damage, Damage);

		FGameplayAbilityTargetDataHandle TargetDataHandle
			= UAbilitySystemBlueprintLibrary::AbilityTargetDataFromActor(TargetActor);

		ApplyGameplayEffectSpecToTarget(Spec->Handle,
		                                ActorInfo,
		                                GetCurrentActivationInfo(),
		                                DamageSpecHandle,
		                                TargetDataHandle);
	}
}
