// Fill out your copyright notice in the Description page of Project Settings.


#include "ITItemManagerComponent.h"
#include "ITItemDefinition.h"
#include "AbilitySystem/ITAbilitySystemComponent.h"
#include "Fragment/ITItemFragment_Equipment.h"
#include "Net/UnrealNetwork.h"
#include "Player/ITPlayerState.h"

UITItemManagerComponent::UITItemManagerComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	SetIsReplicatedByDefault(true);
}

void UITItemManagerComponent::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ThisClass, CurrentHelmetTier);
	DOREPLIFETIME(ThisClass, CurrentArmorTier);
}

bool UITItemManagerComponent::TryAddItem(UITItemDefinition* ItemDefinition)
{
	if (GetOwner()->HasAuthority())
	{
		if (!ItemDefinition)
		{
			return false;
		}

		// 프래그먼트로 장비인지 확인, 프래그먼트가 없을 경우 nullptr을 반환
		if (const UITItemFragment_Equipment* EquipmentFragment
			= ItemDefinition->FindFragmentByClass<UITItemFragment_Equipment>())
		{
			return PutOnEquipment(ItemDefinition);
		}
	}

	return false;
}

bool UITItemManagerComponent::PutOnEquipment(UITItemDefinition* ItemDefinition)
{
	const UITItemFragment_Equipment* EquipmentFragment
		= ItemDefinition->FindFragmentByClass<UITItemFragment_Equipment>();
	if (!EquipmentFragment || !EquipmentFragment->EquipmentEffect)
	{
		return false;
	}

	AITPlayerState* PlayerState = Cast<AITPlayerState>(GetOwner());
	if (!PlayerState)
	{
		return false;
	}

	UITAbilitySystemComponent* AbilitySystemComponent = PlayerState->GetITAbilitySystemComponent();
	if (!AbilitySystemComponent)
	{
		return false;
	}

	int32* CurrentEquipmentTier = nullptr;
	FActiveGameplayEffectHandle* CurrentEffectHandle = nullptr;

	if (EquipmentFragment->EquipmentType == EEquipmentType::Armor)
	{
		CurrentEquipmentTier = &CurrentArmorTier;
		CurrentEffectHandle = &ArmorEffectHandle;
	}
	else if (EquipmentFragment->EquipmentType == EEquipmentType::Helmet)
	{
		CurrentEquipmentTier = &CurrentHelmetTier;
		CurrentEffectHandle = &HelmetEffectHandle;
	}

	if (*CurrentEquipmentTier >= EquipmentFragment->EquipmentTier)
	{
		return false;
	}

	if (CurrentEffectHandle->IsValid())
	{
		AbilitySystemComponent->RemoveActiveGameplayEffect(*CurrentEffectHandle);
	}

	FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
	const UGameplayEffect* GameplayEffect = EquipmentFragment->EquipmentEffect->GetDefaultObject<UGameplayEffect>();
	FActiveGameplayEffectHandle NewHandle
		= AbilitySystemComponent->ApplyGameplayEffectToSelf(GameplayEffect, 1.f, EffectContext);

	if (NewHandle.IsValid())
	{
		*CurrentEffectHandle = NewHandle;
		*CurrentEquipmentTier = EquipmentFragment->EquipmentTier;
		return true;
	}
	return false;
}
