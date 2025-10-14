// Fill out your copyright notice in the Description page of Project Settings.


#include "ITReloadAbility.h"

#include "ITItemDefinition_Weapon.h"
#include "ITWeaponManagerComponent.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "AbilitySystem/ITAbilitySystemComponent.h"
#include "AbilitySystem/Attributes/ITAmmoSet.h"
#include "Character/ITCharacter.h"
#include "Components/AudioComponent.h"
#include "Item/ITItemGameplayTags.h"
#include "Item/ITItemInstance.h"
#include "Kismet/GameplayStatics.h"
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

	UITAbilitySystemComponent* AbilitySystemComponent = PlayerState->GetITAbilitySystemComponent();
	UITWeaponManagerComponent* WeaponManagerComponent = PlayerState->GetITWeaponManagerComponent();
	if (!AbilitySystemComponent || !WeaponManagerComponent)
	{
		return false;
	}

	UITItemInstance* ItemInstance = WeaponManagerComponent->GetCurrentWeapon();
	if (!ItemInstance)
	{
		return false;
	}

	UITItemDefinition* ItemDefinition = ItemInstance->GetItemDefinition();
	if (!ItemDefinition)
	{
		return false;
	}

	float ReserveAmmo = 0.0f;
	if (ItemDefinition->AmmoType == EAmmoType::NormalAmmo)
	{
		ReserveAmmo = AbilitySystemComponent->GetNumericAttribute(UITAmmoSet::GetNormalAmmoAttribute());
	}
	else if (ItemDefinition->AmmoType == EAmmoType::SpecialAmmo)
	{
		ReserveAmmo = AbilitySystemComponent->GetNumericAttribute(UITAmmoSet::GetSpecialAmmoAttribute());
	}

	if (ReserveAmmo <= 0)
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

	if (ActorInfo->IsLocallyControlled() && ReloadSound)
	{
		if (ReloadAudioComponent)
		{
			ReloadAudioComponent->Stop();
		}

		if (AActor* AvatarActor = GetAvatarActorFromActorInfo())
		{
			if (AITCharacter* ITCharacter = Cast<AITCharacter>(AvatarActor))
			{
				ReloadAudioComponent = UGameplayStatics::SpawnSoundAttached(ReloadSound, ITCharacter->GetMesh());
			}
		}
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
	if (ActorInfo->IsLocallyControlled() && ReloadSound)
	{
		if (ReloadAudioComponent && ReloadAudioComponent->IsActive())
		{
			ReloadAudioComponent->Stop();
			ReloadAudioComponent = nullptr;
		}
	}

	MontageStop();
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UITReloadAbility::OnMontageCompleted()
{
	const FGameplayAbilityActorInfo* ActorInfo = GetCurrentActorInfo();
	AITPlayerState* PlayerState = Cast<AITPlayerState>(ActorInfo->OwnerActor.Get());
	if (!PlayerState || !PlayerState->HasAuthority())
	{
		return;
	}

	UITAbilitySystemComponent* AbilitySystemComponent = PlayerState->GetITAbilitySystemComponent();
	UITWeaponManagerComponent* WeaponManagerComponent = PlayerState->GetITWeaponManagerComponent();
	if (!AbilitySystemComponent || !WeaponManagerComponent)
	{
		return;
	}


	UITItemInstance* ItemInstance = WeaponManagerComponent->GetCurrentWeapon();
	if (!ItemInstance)
	{
		return;
	}

	UITItemDefinition* ItemDefinition = ItemInstance->GetItemDefinition();
	if (!ItemDefinition)
	{
		return;
	}

	float ReserveAmmo = 0.0f;
	if (ItemDefinition->AmmoType == EAmmoType::NormalAmmo)
	{
		ReserveAmmo = AbilitySystemComponent->GetNumericAttribute(UITAmmoSet::GetNormalAmmoAttribute());
	}
	else if (ItemDefinition->AmmoType == EAmmoType::SpecialAmmo)
	{
		ReserveAmmo = AbilitySystemComponent->GetNumericAttribute(UITAmmoSet::GetSpecialAmmoAttribute());
	}

	const float Ammo = AbilitySystemComponent->GetNumericAttribute(UITAmmoSet::GetAmmoAttribute());
	const float MaxAmmo = AbilitySystemComponent->GetNumericAttribute(UITAmmoSet::GetMaxAmmoAttribute());
	const int32 NeededAmmo = FMath::FloorToInt(MaxAmmo - Ammo);
	const int32 AmmoToReload = FMath::Min(NeededAmmo, FMath::FloorToInt(ReserveAmmo));

	if (AmmoToReload > 0)
	{
		if (ItemDefinition->AmmoType == EAmmoType::NormalAmmo)
		{
			const float CurrentNormalAmmo
				= AbilitySystemComponent->GetNumericAttribute(UITAmmoSet::GetNormalAmmoAttribute());
			AbilitySystemComponent->SetNumericAttributeBase(
				UITAmmoSet::GetNormalAmmoAttribute(), CurrentNormalAmmo - AmmoToReload);
		}
		else if (ItemDefinition->AmmoType == EAmmoType::SpecialAmmo)
		{
			const float CurrentSpecialAmmo
				= AbilitySystemComponent->GetNumericAttribute(UITAmmoSet::GetSpecialAmmoAttribute());
			AbilitySystemComponent->SetNumericAttributeBase(
				UITAmmoSet::GetSpecialAmmoAttribute(), CurrentSpecialAmmo - AmmoToReload);
		}

		if (ReloadEffectClass)
		{
			FGameplayEffectSpecHandle ReloadSpecHandle = MakeOutgoingGameplayEffectSpec(ReloadEffectClass);
			if (ReloadSpecHandle.IsValid())
			{
				ReloadSpecHandle.Data->SetSetByCallerMagnitude(
					ITItemGameplayTags::Ammo_Reload, static_cast<float>(AmmoToReload));
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
