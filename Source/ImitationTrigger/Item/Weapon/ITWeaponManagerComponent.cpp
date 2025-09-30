// Fill out your copyright notice in the Description page of Project Settings.


#include "ITWeaponManagerComponent.h"
#include "ITItemDefinition_Weapon.h"
#include "Item/ITItemInstance.h"
#include "Player/ITPlayerState.h"
#include "AbilitySystem/ITAbilitySystemComponent.h"
#include "AbilitySystem/Attributes/ITAmmoSet.h"
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

	DOREPLIFETIME(ThisClass, CurrentWeaponType);
	DOREPLIFETIME(ThisClass, MainWeaponInstance);
	DOREPLIFETIME(ThisClass, SubWeaponInstance);
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
		SetMainWeaponInstance(NewWeaponInstance);
		if (CurrentWeaponType == ECurrentWeaponSlot::None)
		{
			SetCurrentWeaponType(ECurrentWeaponSlot::MainWeapon);
			EquipWeapon();
			OnCurrentWeaponTypeChanged.Broadcast(CurrentWeaponType);
		}
		return;
	}

	if (SubWeaponInstance == nullptr)
	{
		SetSubWeaponInstance(NewWeaponInstance);
		if (CurrentWeaponType == ECurrentWeaponSlot::None)
		{
			SetCurrentWeaponType(ECurrentWeaponSlot::SubWeapon);
			EquipWeapon();
			OnCurrentWeaponTypeChanged.Broadcast(CurrentWeaponType);
		}
		return;
	}

	if (MainWeaponInstance && SubWeaponInstance)
	{
		ServerRPC_ChangeWeapon(ECurrentWeaponSlot::MainWeapon);
	}

	ServerRPC_DropCurrentWeapon();
	if (MainWeaponInstance == nullptr)
	{
		SetMainWeaponInstance(NewWeaponInstance);
		SetCurrentWeaponType(ECurrentWeaponSlot::MainWeapon);
		OnCurrentWeaponTypeChanged.Broadcast(CurrentWeaponType);
	}
	else if (SubWeaponInstance == nullptr)
	{
		SetSubWeaponInstance(NewWeaponInstance);
		SetCurrentWeaponType(ECurrentWeaponSlot::SubWeapon);
		OnCurrentWeaponTypeChanged.Broadcast(CurrentWeaponType);
	}
	EquipWeapon();
}

void UITWeaponManagerComponent::ServerRPC_ChangeWeapon_Implementation(ECurrentWeaponSlot WeaponToChange)
{
	if (!GetOwner()->HasAuthority())
	{
		return;
	}

	UnequipWeapon();
	SetCurrentWeaponType(WeaponToChange);
	if (GetCurrentWeapon() == nullptr)
	{
		CurrentWeaponType = ECurrentWeaponSlot::None;
	}
	EquipWeapon();
	OnCurrentWeaponTypeChanged.Broadcast(CurrentWeaponType);
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

	ECurrentWeaponSlot WeaponTypeToDrop = CurrentWeaponType;
	UnequipWeapon();
	if (WeaponTypeToDrop == ECurrentWeaponSlot::MainWeapon)
	{
		SetMainWeaponInstance(nullptr);
	}
	else if (WeaponTypeToDrop == ECurrentWeaponSlot::SubWeapon)
	{
		SetSubWeaponInstance(nullptr);
	}
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

	FGameplayAttribute MaxAmmoAttribute = UITAmmoSet::GetMaxAmmoAttribute();
	FGameplayAttribute AmmoAttribute = UITAmmoSet::GetAmmoAttribute();
	AbilitySystemComponent->SetNumericAttributeBase(MaxAmmoAttribute, WeaponDefinition->MaxAmmo);
	AbilitySystemComponent->SetNumericAttributeBase(AmmoAttribute, WeaponToEquip->CurrentAmmo);

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

	FGameplayAttribute AmmoAttribute = UITAmmoSet::GetAmmoAttribute();
	FGameplayAttribute MaxAmmoAttribute = UITAmmoSet::GetMaxAmmoAttribute();
	if (GetCurrentWeapon() != nullptr)
	{
		GetCurrentWeapon()->CurrentAmmo = AbilitySystemComponent->GetNumericAttributeBase(AmmoAttribute);
	}
	AbilitySystemComponent->SetNumericAttributeBase(AmmoAttribute, 0);
	AbilitySystemComponent->SetNumericAttributeBase(MaxAmmoAttribute, 0);

	if (WeaponPartHandle.IsValid())
	{
		CharacterPartComponent->RemoveCharacterPart(WeaponPartHandle);
		WeaponPartHandle.Reset();
	}

	GrantedHandles.TakeFromAbilitySystem(AbilitySystemComponent);
	SetCurrentWeaponType(ECurrentWeaponSlot::None);
	OnCurrentWeaponTypeChanged.Broadcast(CurrentWeaponType);
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

void UITWeaponManagerComponent::SetCurrentWeaponType(ECurrentWeaponSlot InType)
{
	CurrentWeaponType = InType;
}

void UITWeaponManagerComponent::SetMainWeaponInstance(TObjectPtr<UITItemInstance> WeaponInstance)
{
	MainWeaponInstance = WeaponInstance;
	OnMainWeaponChanged.Broadcast(MainWeaponInstance);
}

void UITWeaponManagerComponent::SetSubWeaponInstance(TObjectPtr<UITItemInstance> WeaponInstance)
{
	SubWeaponInstance = WeaponInstance;
	OnSubWeaponChanged.Broadcast(SubWeaponInstance);
}
