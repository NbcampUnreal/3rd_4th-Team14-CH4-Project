// Fill out your copyright notice in the Description page of Project Settings.


#include "ITWeaponFireAbility_HitscanSG.h"
#include "AbilitySystemComponent.h"
#include "ITItemDefinition_Weapon.h"
#include "Character/ITCharacter.h"
#include "Item/ITItemGameplayTags.h"
#include "Player/ITPlayerState.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"

void UITWeaponFireAbility_HitscanSG::Fire(const FVector& TraceStart, const FVector& TraceDirection)
{
	Super::Fire(TraceStart, TraceDirection);

	UITItemDefinition_Weapon* WeaponDefinition = GetWeaponDefinition();
	if (!WeaponDefinition)
	{
		return;
	}

	float EffectiveRange = WeaponDefinition->EffectiveRange;
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(GetAvatarActorFromActorInfo());

	for (int32 i = 0; i < 10; i++)
	{
		FVector RandomDirection = UKismetMathLibrary::RandomUnitVectorInConeInRadians(TraceDirection, 0.05f);
		FVector TraceEnd = TraceStart + (RandomDirection * EffectiveRange);
		FHitResult HitResult;

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
				if (AITPlayerState* PlayerState = Player->GetITPlayerState())
				{
					if (PlayerState->bIsAlive)
					{
						if (UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo())
						{
							FGameplayCueParameters CueParams;
							CueParams.Location = HitResult.ImpactPoint;
							CueParams.Normal = HitResult.Normal;
							ASC->ExecuteGameplayCue(ITItemGameplayTags::GameplayCue_Weapon_Hit_Player, CueParams);
						}
					}
				}
			}
			else
			{
				if (UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo())
				{
					FGameplayCueParameters CueParams;
					CueParams.Location = HitResult.ImpactPoint;
					CueParams.Normal = HitResult.Normal;
					ASC->ExecuteGameplayCue(ITItemGameplayTags::GameplayCue_Weapon_Hit_Others, CueParams);
				}
			}
			
			ApplyWeaponDamage(HitResult.GetActor());
		}
	}
}
