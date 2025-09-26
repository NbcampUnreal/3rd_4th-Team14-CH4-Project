// Fill out your copyright notice in the Description page of Project Settings.


#include "ITReloadAbility.h"
#include "ITWeaponManagerComponent.h"
#include "AbilitySystem/ITAbilitySystemComponent.h"
#include "AbilitySystem/Attributes/ITAmmoSet.h"
#include "Item/ITItemGameplayTags.h"
#include "Player/ITPlayerState.h"

UITReloadAbility::UITReloadAbility()
{
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::ServerInitiated;
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

bool UITReloadAbility::CanActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                          const FGameplayAbilityActorInfo* ActorInfo,
                                          const FGameplayTagContainer* SourceTags,
                                          const FGameplayTagContainer* TargetTags,
                                          FGameplayTagContainer* OptionalRelevantTags) const
{
	if (!Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags))
	{
		return false;
	}

	AITPlayerState* PlayerState = Cast<AITPlayerState>(ActorInfo->OwnerActor.Get());
	if (!PlayerState)
	{
		return false;
	}

	UITWeaponManagerComponent* WeaponManagerComponent = PlayerState->GetITWeaponManagerComponent();
	UITAbilitySystemComponent* AbilitySystemComponent = PlayerState->GetITAbilitySystemComponent();
	if (!WeaponManagerComponent || !AbilitySystemComponent)
	{
		return false;
	}

	if (WeaponManagerComponent->GetReserveAmmo() <= 0)
	{
		return false;
	}

	const float Ammo = AbilitySystemComponent->GetNumericAttribute(UITAmmoSet::GetAmmoAttribute());
	const float MaxAmmo = AbilitySystemComponent->GetNumericAttribute(UITAmmoSet::GetMaxAmmoAttribute());
	if (Ammo >= MaxAmmo)
	{
		return false;
	}

	return true;
}

void UITReloadAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
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

	AITPlayerState* PlayerState = Cast<AITPlayerState>(ActorInfo->OwnerActor.Get());
	if (!PlayerState)
	{
		return;
	}

	UITWeaponManagerComponent* WeaponManagerComponent = PlayerState->GetITWeaponManagerComponent();
	UITAbilitySystemComponent* AbilitySystemComponent = PlayerState->GetITAbilitySystemComponent();
	if (!WeaponManagerComponent || !AbilitySystemComponent)
	{
		return;
	}

	const float Ammo = AbilitySystemComponent->GetNumericAttribute(UITAmmoSet::GetAmmoAttribute());
	const float MaxAmmo = AbilitySystemComponent->GetNumericAttribute(UITAmmoSet::GetMaxAmmoAttribute());
	const int32 NeededAmmo = FMath::FloorToInt(MaxAmmo - Ammo);
	const int32 ReserveAmmo = WeaponManagerComponent->GetReserveAmmo();
	const int32 AmmoToReload = FMath::Min(NeededAmmo, ReserveAmmo);

	if (AmmoToReload > 0)
	{
		WeaponManagerComponent->ServerRPC_ConsumeAmmo(AmmoToReload);

		if (ReloadEffectClass)
		{
			FGameplayEffectSpecHandle ReloadSpecHandle = MakeOutgoingGameplayEffectSpec(ReloadEffectClass);
			if (ReloadSpecHandle.IsValid())
			{
				ReloadSpecHandle.Data->SetSetByCallerMagnitude(
					ITItemGameplayTags::Item_Ammo, static_cast<float>(AmmoToReload));
				ApplyGameplayEffectSpecToOwner(Handle, ActorInfo, ActivationInfo, ReloadSpecHandle);
			}
		}
	}

	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}
