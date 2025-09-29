// Fill out your copyright notice in the Description page of Project Settings.


#include "ITReloadAbility.h"
#include "ITWeaponManagerComponent.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
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

	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	if (!ReloadMontage)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
		return;
	}

	UAbilityTask_PlayMontageAndWait* ReloadMontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
		this, NAME_None, ReloadMontage, 1.0f, NAME_None, false);

	if (ReloadMontageTask)
	{
		ReloadMontageTask->OnCompleted.AddDynamic(this, &UITReloadAbility::OnMontageCompleted);
		ReloadMontageTask->OnCancelled.AddDynamic(this, &UITReloadAbility::OnMontageCancelled);
		ReloadMontageTask->OnInterrupted.AddDynamic(this, &UITReloadAbility::OnMontageCancelled);
		ReloadMontageTask->ReadyForActivation();
	}
	else
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
	}
}

void UITReloadAbility::EndAbility(const FGameplayAbilitySpecHandle Handle,
                                  const FGameplayAbilityActorInfo* ActorInfo,
                                  const FGameplayAbilityActivationInfo ActivationInfo,
                                  bool bReplicateEndAbility,
                                  bool bWasCancelled)
{
	MontageStop();
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UITReloadAbility::OnMontageCompleted()
{
	const FGameplayAbilityActorInfo* ActorInfo = GetCurrentActorInfo();
	AITPlayerState* PlayerState = Cast<AITPlayerState>(ActorInfo->OwnerActor.Get());
	if (!PlayerState || !PlayerState->HasAuthority())
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
		return;
	}

	UITWeaponManagerComponent* WeaponManagerComponent = PlayerState->GetITWeaponManagerComponent();
	UITAbilitySystemComponent* AbilitySystemComponent = PlayerState->GetITAbilitySystemComponent();
	if (!WeaponManagerComponent || !AbilitySystemComponent)
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
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
				ApplyGameplayEffectSpecToOwner(
					CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, ReloadSpecHandle);
			}
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("재장전 성공"));
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UITReloadAbility::OnMontageCancelled()
{
	UE_LOG(LogTemp, Warning, TEXT("재장전 취소"));
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}
