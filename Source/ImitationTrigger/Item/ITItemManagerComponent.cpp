// Fill out your copyright notice in the Description page of Project Settings.


#include "ITItemManagerComponent.h"
#include "ITItemActor.h"
#include "ITItemDefinition.h"
#include "ITItemInstance.h"
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

void UITItemManagerComponent::OnRep_CurrentHelmetChanged()
{
	OnCurrentHelmetChanged.Broadcast(CurrentHelmetTier);
}

void UITItemManagerComponent::OnRep_CurrentArmorChanged()
{
	OnCurrentArmorChanged.Broadcast(CurrentArmorTier);
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

	FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();

	if (EquipmentFragment->EquipmentType == EEquipmentType::Armor)
	{
		if (CurrentArmorTier >= EquipmentFragment->EquipmentTier)
		{
			return false;
		}

		if (ArmorDefinition)
		{
			DropEquipment(ArmorDefinition);
		}

		if (ArmorEffectHandle.IsValid())
		{
			AbilitySystemComponent->RemoveActiveGameplayEffect(ArmorEffectHandle);
		}

		const UGameplayEffect* GameplayEffect = EquipmentFragment->EquipmentEffect->GetDefaultObject<UGameplayEffect>();
		FActiveGameplayEffectHandle NewHandle
			= AbilitySystemComponent->ApplyGameplayEffectToSelf(GameplayEffect, 1.f, EffectContext);

		if (NewHandle.IsValid())
		{
			CurrentArmorTier = EquipmentFragment->EquipmentTier;
			ArmorEffectHandle = NewHandle;
			ArmorDefinition = ItemDefinition;
			return true;
		}
	}
	else if (EquipmentFragment->EquipmentType == EEquipmentType::Helmet)
	{
		if (CurrentHelmetTier >= EquipmentFragment->EquipmentTier)
		{
			return false;
		}

		if (HelmetDefinition)
		{
			DropEquipment(HelmetDefinition);
		}

		if (HelmetEffectHandle.IsValid())
		{
			AbilitySystemComponent->RemoveActiveGameplayEffect(HelmetEffectHandle);
		}

		const UGameplayEffect* GameplayEffect = EquipmentFragment->EquipmentEffect->GetDefaultObject<UGameplayEffect>();
		FActiveGameplayEffectHandle NewHandle
			= AbilitySystemComponent->ApplyGameplayEffectToSelf(GameplayEffect, 1.f, EffectContext);

		if (NewHandle.IsValid())
		{
			CurrentHelmetTier = EquipmentFragment->EquipmentTier;
			HelmetEffectHandle = NewHandle;
			HelmetDefinition = ItemDefinition;
			return true;
		}
	}
	return false;
}

void UITItemManagerComponent::DropEquipment(UITItemDefinition* ItemDefinition)
{
	AITPlayerState* PlayerState = Cast<AITPlayerState>(GetOwner());
	if (!PlayerState)
	{
		return;
	}

	APawn* Pawn = PlayerState->GetPawn();
	if (!Pawn)
	{
		return;
	}

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride =
		ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	AITItemActor* DroppedItemActor = GetWorld()->SpawnActor<AITItemActor>(
		AITItemActor::StaticClass(),
		Pawn->GetActorLocation() + FVector(0, 0, 50),
		FRotator::ZeroRotator,
		SpawnParams
	);

	if (DroppedItemActor)
	{
		UITItemInstance* NewItemInstance = NewObject<UITItemInstance>(DroppedItemActor);
		NewItemInstance->InitItemInstance(ItemDefinition);
		DroppedItemActor->InitItemActor(NewItemInstance);
	}
}
