// Fill out your copyright notice in the Description page of Project Settings.


#include "ITWeaponManagerComponent.h"
#include "ITItemDefinition_Weapon.h"
#include "Item/ITItemInstance.h"
#include "Player/ITPlayerState.h"
#include "AbilitySystem/ITAbilitySystemComponent.h"
#include "Cosmetics/ITCharacterPartComponent.h"
#include "Item/ITItemActor.h"
#include "Engine/ActorChannel.h"
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

bool UITWeaponManagerComponent::ReplicateSubobjects(UActorChannel* Channel, FOutBunch* Bunch, FReplicationFlags* RepFlags)
{
	bool bWroteSomething = Super::ReplicateSubobjects(Channel, Bunch, RepFlags);
	// TODO: MainWeaponInstance, SubWeaponInstance의 Replication 문제 해결
	// 아래 방식으로 하면, Replicated는 되는데, 아이템을 버릴 때 문제가 발생함 (아이템이 막 복사됨...)
	//if (IsValid(MainWeaponInstance))
	//{
	//	bWroteSomething |= Channel->ReplicateSubobject(MainWeaponInstance, *Bunch, *RepFlags);
	//}
	//if (IsValid(SubWeaponInstance))
	//{
	//	bWroteSomething |= Channel->ReplicateSubobject(SubWeaponInstance, *Bunch, *RepFlags);
	//}
	return bWroteSomething;
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
	SetCurrentWeaponType(TargetSlot);
	EquipWeapon();
	OnCurrentWeaponTypeChanged.Broadcast(CurrentWeaponType);
}

void UITWeaponManagerComponent::ServerRPC_ChangeWeapon_Implementation(ECurrentWeaponSlot WeaponToChange)
{
	if (!GetOwner()->HasAuthority())
	{
		return;
	}

	UnequipWeapon();
	SetCurrentWeaponType(WeaponToChange);
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

	if (CurrentWeaponType == ECurrentWeaponSlot::MainWeapon)
	{
		SetMainWeaponInstance(nullptr);
	}
	else if (CurrentWeaponType == ECurrentWeaponSlot::SubWeapon)
	{
		SetSubWeaponInstance(nullptr);
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

	SetCurrentWeaponType(PreviousWeaponType);
	PreviousWeaponType = ECurrentWeaponSlot::None;
	EquipWeapon();
	OnCurrentWeaponTypeChanged.Broadcast(CurrentWeaponType);
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
