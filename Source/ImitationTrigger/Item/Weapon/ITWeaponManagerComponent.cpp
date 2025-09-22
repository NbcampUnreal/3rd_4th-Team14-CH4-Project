// Fill out your copyright notice in the Description page of Project Settings.


#include "ITWeaponManagerComponent.h"
#include "ITItemDefinition_Weapon.h"
#include "Item/ITItemInstance.h"
#include "Player/ITPlayerState.h"
#include "AbilitySystem/ITAbilitySystemComponent.h"
#include "Cosmetics/ITCharacterPartComponent.h"
#include "Item/ITItemActor.h"
#include "Net/UnrealNetwork.h"

UITWeaponManagerComponent::UITWeaponManagerComponent()
{
	SetIsReplicatedByDefault(true);
}

void UITWeaponManagerComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UITWeaponManagerComponent, CurrentWeapon);
	DOREPLIFETIME(UITWeaponManagerComponent, MainWeaponInstance);
	DOREPLIFETIME(UITWeaponManagerComponent, SubWeaponInstance);
}

void UITWeaponManagerComponent::OnRep_CurrentWeaponChanged()
{
	OnCurrentWeaponChanged.Broadcast(CurrentWeapon);
}

void UITWeaponManagerComponent::OnRep_CurrentMainWeaponChanged()
{
	OnMainWeaponChanged.Broadcast(MainWeaponInstance);
}

void UITWeaponManagerComponent::OnRep_CurrentSubWeaponChanged()
{
	OnSubWeaponChanged.Broadcast(SubWeaponInstance);
}

void UITWeaponManagerComponent::ServerRPC_PickupWeapon_Implementation(UITItemInstance* NewWeaponInstance)
{
	if (!GetOwner()->HasAuthority() || !NewWeaponInstance)
	{
		return;
	}

	if (MainWeaponInstance == nullptr)
	{
		MainWeaponInstance = NewWeaponInstance;
		if (CurrentWeapon == ECurrentWeaponSlot::None)
		{
			CurrentWeapon = ECurrentWeaponSlot::MainWeapon;
			EquipWeapon();
		}
		return;
	}

	if (SubWeaponInstance == nullptr)
	{
		SubWeaponInstance = NewWeaponInstance;
		if (CurrentWeapon == ECurrentWeaponSlot::None)
		{
			CurrentWeapon = ECurrentWeaponSlot::SubWeapon;
			EquipWeapon();
		}
		return;
	}

	ServerRPC_DropCurrentWeapon();
	if (MainWeaponInstance == nullptr)
	{
		MainWeaponInstance = NewWeaponInstance;
		CurrentWeapon = ECurrentWeaponSlot::MainWeapon;
	}
	else if (SubWeaponInstance == nullptr)
	{
		SubWeaponInstance = NewWeaponInstance;
		CurrentWeapon = ECurrentWeaponSlot::SubWeapon;
	}
	EquipWeapon();
}

void UITWeaponManagerComponent::ServerRPC_SwapWeapon_Implementation()
{
	if (!GetOwner()->HasAuthority())
	{
		return;
	}

	ECurrentWeaponSlot TargetSlot = ECurrentWeaponSlot::None;
	if (CurrentWeapon == ECurrentWeaponSlot::MainWeapon && SubWeaponInstance != nullptr)
	{
		TargetSlot = ECurrentWeaponSlot::SubWeapon;
	}
	else if (CurrentWeapon == ECurrentWeaponSlot::SubWeapon && MainWeaponInstance != nullptr)
	{
		TargetSlot = ECurrentWeaponSlot::MainWeapon;
	}
	else if (CurrentWeapon == ECurrentWeaponSlot::None && MainWeaponInstance != nullptr)
	{
		TargetSlot = ECurrentWeaponSlot::MainWeapon;
	}
	else if (CurrentWeapon == ECurrentWeaponSlot::None && SubWeaponInstance != nullptr)
	{
		TargetSlot = ECurrentWeaponSlot::SubWeapon;
	}

	UnequipWeapon();
	CurrentWeapon = TargetSlot;
	EquipWeapon();
}

void UITWeaponManagerComponent::ServerRPC_DropCurrentWeapon_Implementation()
{
	if (!GetOwner()->HasAuthority() || CurrentWeapon == ECurrentWeaponSlot::None)
	{
		return;
	}

	AITPlayerState* PlayerState = GetOwner<AITPlayerState>();
	if (!PlayerState)
	{
		return;
	}

	if (GetWorld())
	{
		FVector DropLocation
			= PlayerState->GetPawn()->GetActorLocation() + PlayerState->GetPawn()->GetActorForwardVector() * 300.0f;
		FRotator DropRotation = FRotator::ZeroRotator;
		FActorSpawnParameters SpawnParameters;
		SpawnParameters.SpawnCollisionHandlingOverride
			= ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

		AITItemActor* WeaponActorToDrop = GetWorld()->SpawnActor<AITItemActor>(
			AITItemActor::StaticClass(), DropLocation, DropRotation, SpawnParameters);

		if (WeaponActorToDrop)
		{
			WeaponActorToDrop->InitItemActor(GetCurrentWeapon());
		}
	}

	if (CurrentWeapon == ECurrentWeaponSlot::MainWeapon)
	{
		MainWeaponInstance = nullptr;
	}
	else if (CurrentWeapon == ECurrentWeaponSlot::SubWeapon)
	{
		SubWeaponInstance = nullptr;
	}

	UnequipWeapon();
}

void UITWeaponManagerComponent::EquipWeapon()
{
	UITItemInstance* WeaponToEquip = GetCurrentWeapon();
	if (!WeaponToEquip)
	{
		return;
	}

	UITItemDefinition_Weapon* WeaponDefinition
		= Cast<UITItemDefinition_Weapon>(WeaponToEquip->GetItemDefinition());
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

	UITCharacterPartComponent* CharacterPartComponent
		= PlayerState->GetPawn()->FindComponentByClass<UITCharacterPartComponent>();
	if (!CharacterPartComponent)
	{
		return;
	}

	WeaponPartHandle = CharacterPartComponent->AddCharacterPart(WeaponDefinition->WeaponPart);

	if (WeaponDefinition->WeaponAbilitySet)
	{
		WeaponDefinition->WeaponAbilitySet->GiveToAbilitySystem(
			AbilitySystemComponent, &GrantedHandles, WeaponToEquip);
	}
}

void UITWeaponManagerComponent::UnequipWeapon()
{
	if (CurrentWeapon == ECurrentWeaponSlot::None)
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

	UITCharacterPartComponent* CharacterPartComponent
		= PlayerState->GetPawn()->FindComponentByClass<UITCharacterPartComponent>();
	if (!CharacterPartComponent)
	{
		return;
	}

	if (WeaponPartHandle.IsValid())
	{
		CharacterPartComponent->RemoveCharacterPart(WeaponPartHandle);
		WeaponPartHandle.Reset();
	}

	GrantedHandles.TakeFromAbilitySystem(AbilitySystemComponent);
	CurrentWeapon = ECurrentWeaponSlot::None;
}

UITItemInstance* UITWeaponManagerComponent::GetCurrentWeapon() const
{
	if (CurrentWeapon == ECurrentWeaponSlot::MainWeapon)
	{
		return MainWeaponInstance;
	}

	if (CurrentWeapon == ECurrentWeaponSlot::SubWeapon)
	{
		return SubWeaponInstance;
	}

	return nullptr;
}
