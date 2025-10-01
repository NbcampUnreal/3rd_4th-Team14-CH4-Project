// Fill out your copyright notice in the Description page of Project Settings.


#include "ITWeaponFireAbility_Hitscan.h"

#include "AbilitySystemComponent.h"
#include "ITItemDefinition_Weapon.h"
#include "Character/ITCharacter.h"
#include "Item/ITItemGameplayTags.h"
#include "Kismet/KismetSystemLibrary.h"

void UITWeaponFireAbility_Hitscan::Fire(const FVector& TraceStart, const FVector& TraceDirection)
{
	Super::Fire(TraceStart, TraceDirection);

	UITItemDefinition_Weapon* WeaponDefinition = GetWeaponDefinition();
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
		this, TraceStart, TraceEnd,
		UEngineTypes::ConvertToTraceType(ECC_Visibility),
		false, ActorsToIgnore,
		EDrawDebugTrace::ForDuration,
		HitResult, true,
		FLinearColor::White, FLinearColor::Red, 1.0f
	);

	if (HitResult.bBlockingHit && HitResult.GetActor())
	{
		if (AITCharacter* Player = Cast<AITCharacter>(HitResult.GetActor()))
		{
			if (UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo())
			{
				FGameplayCueParameters CueParams;
				CueParams.Location = HitResult.ImpactPoint;
				CueParams.Normal = HitResult.Normal;
				ASC->ExecuteGameplayCue(ITItemGameplayTags::GameplayCue_Weapon_Hit, CueParams);
			}
		}
		ApplyWeaponDamage(HitResult.GetActor());
	}
}
