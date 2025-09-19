// Fill out your copyright notice in the Description page of Project Settings.


#include "ITWeaponManagerComponent.h"
#include "Item/ITItemDefinition_Weapon.h"
#include "Item/ITItemInstance.h"
#include "Player/ITPlayerState.h"
#include "AbilitySystem/ITAbilitySystemComponent.h"
#include "Cosmetics/ITCharacterPartComponent.h"
#include "Net/UnrealNetwork.h"

UITWeaponManagerComponent::UITWeaponManagerComponent()
{
	SetIsReplicatedByDefault(true);
}

void UITWeaponManagerComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UITWeaponManagerComponent, PrimaryWeaponInstance);
	DOREPLIFETIME(UITWeaponManagerComponent, SecondaryWeaponInstance);
}

void UITWeaponManagerComponent::ServerRPC_EquipWeapon_Implementation(UITItemInstance* WeaponInstance)
{
	if (GetOwner()->HasAuthority() && WeaponInstance)
	{
		UITItemDefinition_Weapon* WeaponDefinition
			= Cast<UITItemDefinition_Weapon>(WeaponInstance->GetItemDefinition());
		if (!WeaponDefinition)
		{
			return;
		}

		AITPlayerState* PlayerState = GetOwner<AITPlayerState>();
		if (!PlayerState)
		{
			return;
		}

		UITAbilitySystemComponent* AbilitySystemComponent = PlayerState->GetITAbilitySystemComponent();
		if (!AbilitySystemComponent)
		{
			return;
		}

		UITCharacterPartComponent* CPC = PlayerState->GetPawn()->FindComponentByClass<UITCharacterPartComponent>();
		if (!CPC)
		{
			return;
		}

		WeaponPartHandle = CPC->AddCharacterPart(WeaponDefinition->WeaponPart);

		ServerRPC_UnequipWeapon();

		if (WeaponDefinition->WeaponAbilitySet)
		{
			WeaponDefinition->WeaponAbilitySet->GiveToAbilitySystem(
				AbilitySystemComponent, &GrantedHandles, WeaponInstance);
		}

		PrimaryWeaponInstance = WeaponInstance;
	}
}

void UITWeaponManagerComponent::ServerRPC_UnequipWeapon_Implementation()
{
	if (GetOwner()->HasAuthority() && PrimaryWeaponInstance)
	{
		AITPlayerState* PlayerState = GetOwner<AITPlayerState>();
		if (!PlayerState)
		{
			return;
		}

		UITAbilitySystemComponent* AbilitySystemComponent = PlayerState->GetITAbilitySystemComponent();
		if (!AbilitySystemComponent)
		{
			return;
		}

		UITCharacterPartComponent* CPC = PlayerState->GetPawn()->FindComponentByClass<UITCharacterPartComponent>();
		if (!CPC)
		{
			return;
		}

		if (WeaponPartHandle.IsValid())
		{
			CPC->RemoveCharacterPart(WeaponPartHandle);
		}

		GrantedHandles.TakeFromAbilitySystem(AbilitySystemComponent);
		PrimaryWeaponInstance = nullptr;
	}
}
