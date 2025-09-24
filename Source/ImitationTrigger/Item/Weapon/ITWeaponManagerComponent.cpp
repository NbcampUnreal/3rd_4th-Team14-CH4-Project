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

	DOREPLIFETIME(UITWeaponManagerComponent, CurrentWeaponType);
	DOREPLIFETIME(UITWeaponManagerComponent, MainWeaponInstance);
	DOREPLIFETIME(UITWeaponManagerComponent, SubWeaponInstance);
}

void UITWeaponManagerComponent::OnRep_CurrentWeaponTypeChanged()
{
	OnCurrentWeaponTypeChanged.Broadcast(CurrentWeaponType);
}

void UITWeaponManagerComponent::OnRep_MainWeaponChanged()
{
	OnMainWeaponChanged.Broadcast(MainWeaponInstance);
}

void UITWeaponManagerComponent::OnRep_SubWeaponChanged()
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
		if (CurrentWeaponType == ECurrentWeaponSlot::None)
		{
			CurrentWeaponType = ECurrentWeaponSlot::MainWeapon;
			EquipWeapon();
		}
		return;
	}

	if (SubWeaponInstance == nullptr)
	{
		SubWeaponInstance = NewWeaponInstance;
		if (CurrentWeaponType == ECurrentWeaponSlot::None)
		{
			CurrentWeaponType = ECurrentWeaponSlot::SubWeapon;
			EquipWeapon();
		}
		return;
	}

	ServerRPC_DropCurrentWeapon();
	if (MainWeaponInstance == nullptr)
	{
		MainWeaponInstance = NewWeaponInstance;
		CurrentWeaponType = ECurrentWeaponSlot::MainWeapon;
	}
	else if (SubWeaponInstance == nullptr)
	{
		SubWeaponInstance = NewWeaponInstance;
		CurrentWeaponType = ECurrentWeaponSlot::SubWeapon;
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
	if (CurrentWeaponType == ECurrentWeaponSlot::MainWeapon && SubWeaponInstance != nullptr)
	{
		TargetSlot = ECurrentWeaponSlot::SubWeapon;
	}
	else if (CurrentWeaponType == ECurrentWeaponSlot::SubWeapon && MainWeaponInstance != nullptr)
	{
		TargetSlot = ECurrentWeaponSlot::MainWeapon;
	}
	else if (CurrentWeaponType == ECurrentWeaponSlot::None && MainWeaponInstance != nullptr)
	{
		TargetSlot = ECurrentWeaponSlot::MainWeapon;
	}
	else if (CurrentWeaponType == ECurrentWeaponSlot::None && SubWeaponInstance != nullptr)
	{
		TargetSlot = ECurrentWeaponSlot::SubWeapon;
	}

	UnequipWeapon();
	CurrentWeaponType = TargetSlot;
	EquipWeapon();
}

void UITWeaponManagerComponent::ServerRPC_ChangeWeapon_Implementation(ECurrentWeaponSlot WeaponToChange)
{
	if (!GetOwner()->HasAuthority())
	{
		return;
	}

	UITItemInstance* TargetWeapon = nullptr;
	if (WeaponToChange == ECurrentWeaponSlot::MainWeapon)
	{
		TargetWeapon = MainWeaponInstance;
	}
	else if (WeaponToChange == ECurrentWeaponSlot::SubWeapon)
	{
		TargetWeapon = SubWeaponInstance;
	}

	UnequipWeapon();
	CurrentWeaponType = WeaponToChange;
	EquipWeapon();
}

void UITWeaponManagerComponent::ServerRPC_DropCurrentWeapon_Implementation()
{
	if (!GetOwner()->HasAuthority() || CurrentWeaponType == ECurrentWeaponSlot::None)
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

	if (CurrentWeaponType == ECurrentWeaponSlot::MainWeapon)
	{
		MainWeaponInstance = nullptr;
	}
	else if (CurrentWeaponType == ECurrentWeaponSlot::SubWeapon)
	{
		SubWeaponInstance = nullptr;
	}

	UnequipWeapon();
}

void UITWeaponManagerComponent::ServerRPC_HolsterWeapon_Implementation()
{
	if (!GetOwner()->HasAuthority() || CurrentWeaponType == ECurrentWeaponSlot::None)
	{
		return;
	}

	PreviousWeaponType = CurrentWeaponType;
	UnequipWeapon();
}

void UITWeaponManagerComponent::ServerRPC_ReEquipWeapon_Implementation()
{
	if (!GetOwner()->HasAuthority() || PreviousWeaponType == ECurrentWeaponSlot::None)
	{
		return;
	}

	CurrentWeaponType = PreviousWeaponType;
	PreviousWeaponType = ECurrentWeaponSlot::None;
	EquipWeapon();
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
	if (CurrentWeaponType == ECurrentWeaponSlot::None)
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
	CurrentWeaponType = ECurrentWeaponSlot::None;
}

UITItemInstance* UITWeaponManagerComponent::GetCurrentWeapon() const
{
	if (CurrentWeaponType == ECurrentWeaponSlot::MainWeapon)
	{
		return MainWeaponInstance;
	}

	if (CurrentWeaponType == ECurrentWeaponSlot::SubWeapon)
	{
		return SubWeaponInstance;
	}

	return nullptr;
}
