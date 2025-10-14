// Fill out your copyright notice in the Description page of Project Settings.


#include "ITWeaponFireAbility.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "ITItemDefinition_Weapon.h"
#include "Character/ITCharacter.h"
#include "Player/ITBattlePlayerController.h"
#include "Player/ITPlayerState.h"
#include "Item/ITItemGameplayTags.h"
#include "Item/ITItemInstance.h"
#include "AbilitySystem/Attributes/ITHealthSet.h"

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

	// TODO: 추후, 총구에서 총알이 나갈 수 있게 수정 / 현재, 카메라 위치에서 발사
	FVector TraceStart;
	FRotator ViewRotation;
	ActorInfo->PlayerController->GetPlayerViewPoint(TraceStart, ViewRotation);
	FVector TraceDirection = ViewRotation.Vector();

	PlayClientHUDAnimationOnFire(AvatarActor);
	Fire(TraceStart, TraceDirection);

	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}

void UITWeaponFireAbility::ApplyCooldown(const FGameplayAbilitySpecHandle Handle,
                                         const FGameplayAbilityActorInfo* ActorInfo,
                                         const FGameplayAbilityActivationInfo ActivationInfo) const
{
	// 쿨다운 적용 로직을 커스터마이징 하기 위해 (Cooldown Effect에 Set By Caller를 사용하기 위함)
	// ApplyCooldown()를 override하고, 의도적으로 Super::ApplyCooldown() 호출을 하지 않음
	return;
}

void UITWeaponFireAbility::Fire(const FVector& StartLocation, const FVector& FireDirection)
{
	if (AITCharacter* Character = Cast<AITCharacter>(GetAvatarActorFromActorInfo()))
	{
		Character->MulticastRPC_PlayFireEffects(ReboundAnimMontage, FireAnimMontage, MatchedSkeleton);
	}
}

// TODO 여기서부터
void UITWeaponFireAbility::ApplyWeaponDamage(AActor* TargetActor, bool bIsHeadshot)
{
	if (!TargetActor || !NormalDamageEffect || !HeadshotDamageEffect || !Cast<AITCharacter>(TargetActor))
	{
		return;
	}

	AITCharacter* ITCharacter = Cast<AITCharacter>(TargetActor);
	AITPlayerState* ITPlayerState = ITCharacter->GetITPlayerState();
	if (IsValid(ITPlayerState) && !ITPlayerState->bIsAlive)
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

	UITItemDefinition_Weapon* WeaponDefinition = GetWeaponDefinition();
	if (!WeaponDefinition)
	{
		return;
	}

	float Damage = WeaponDefinition->Damage;

	if (bIsHeadshot)
	{
		Damage *= HeadshotDamageMultiplier;

		FGameplayEffectSpecHandle DamageSpecHandle = MakeOutgoingGameplayEffectSpec(HeadshotDamageEffect);
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
	else
	{
		FGameplayEffectSpecHandle DamageSpecHandle = MakeOutgoingGameplayEffectSpec(NormalDamageEffect);
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

	AActor* AvaterActor = ActorInfo->AvatarActor.Get();
	PlayClientHUDAnimationOnHit(AvaterActor, TargetActor);
}

UITItemDefinition_Weapon* UITWeaponFireAbility::GetWeaponDefinition() const
{
	FGameplayAbilitySpec* Spec = GetCurrentAbilitySpec();
	if (!Spec)
	{
		return nullptr;
	}

	const UITItemInstance* WeaponInstance = Cast<UITItemInstance>(Spec->SourceObject.Get());
	if (!WeaponInstance)
	{
		return nullptr;
	}

	UITItemDefinition_Weapon* WeaponDefinition = Cast<UITItemDefinition_Weapon>(WeaponInstance->GetItemDefinition());
	return WeaponDefinition;
}

void UITWeaponFireAbility::PlayClientHUDAnimationOnFire(AActor* Attacker)
{
	AITCharacter* ITAttacker = Cast<AITCharacter>(Attacker);
	if (IsValid(ITAttacker))
	{
		AITBattlePlayerController* PlayerController = ITAttacker->GetController<AITBattlePlayerController>();
		if (IsValid(PlayerController))
		{
			PlayerController->ClientRPC_OnFireAnimation();
		}
	}
}

void UITWeaponFireAbility::PlayClientHUDAnimationOnHit(AActor* Attacker, AActor* Target)
{
	AITCharacter* ITAttacker = Cast<AITCharacter>(Attacker);
	AITCharacter* ITTarget = Cast<AITCharacter>(Target);
	if (IsValid(ITAttacker) && IsValid(ITTarget))
	{
		AITBattlePlayerController* PlayerController = ITAttacker->GetController<AITBattlePlayerController>();
		if (IsValid(PlayerController))
		{
			UAbilitySystemComponent* TargetASC = ITTarget->GetAbilitySystemComponent();
			if (IsValid(TargetASC))
			{
				float RemainTargetHealth = TargetASC->GetNumericAttribute(UITHealthSet::GetHealthAttribute());
				if (RemainTargetHealth > 0)
				{
					PlayerController->ClientRPC_PlayHitMarkerAnimation();
				}
				else
				{
					PlayerController->ClientRPC_PlayKillMarkerAnimation();
				}
			}
		}
	}
}
