// Fill out your copyright notice in the Description page of Project Settings.


#include "ITWeaponFireAbility_Hitscan.h"
#include "ITItemDefinition_Weapon.h"
#include "Item/ITItemInstance.h"
#include "Kismet/KismetSystemLibrary.h"

void UITWeaponFireAbility_Hitscan::FireType(const FVector& TraceStart, const FVector& TraceDirection)
{
	Super::FireType(TraceStart, TraceDirection);

	FGameplayAbilitySpec* Spec = GetCurrentAbilitySpec();
	if (!Spec)
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

	float EffectiveRange = WeaponDefinition->EffectiveRange;
	FVector TraceEnd = TraceStart + (TraceDirection * EffectiveRange);
	FHitResult HitResult;
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(GetAvatarActorFromActorInfo());

	UKismetSystemLibrary::LineTraceSingle(
		this,
		TraceStart,
		TraceEnd,
		UEngineTypes::ConvertToTraceType(ECC_Visibility),
		false,
		ActorsToIgnore,
		EDrawDebugTrace::ForDuration,
		HitResult,
		true
	);

	if (HitResult.bBlockingHit && HitResult.GetActor())
	{
		ApplyWeaponDamage(HitResult.GetActor());
	}
}
