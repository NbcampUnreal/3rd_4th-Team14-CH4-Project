// Fill out your copyright notice in the Description page of Project Settings.


#include "ITItemActor.h"
#include "ITItemDefinition.h"
#include "ITItemGameplayTags.h"
#include "ITItemInstance.h"
#include "AbilitySystem/ITAbilitySystemComponent.h"
#include "AbilitySystem/Attributes/ITAmmoSet.h"
#include "Character/ITCharacter.h"
#include "Components/SphereComponent.h"
#include "Net/UnrealNetwork.h"
#include "Player/ITPlayerState.h"
#include "Weapon/ITWeaponManagerComponent.h"

AITItemActor::AITItemActor()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
	SetReplicateMovement(true);

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	SetRootComponent(StaticMeshComponent);
	StaticMeshComponent->SetIsReplicated(true);
	StaticMeshComponent->SetSimulatePhysics(true);
	StaticMeshComponent->SetCollisionProfileName(TEXT("PhysicsActor"));

	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	SphereComponent->SetupAttachment(RootComponent);
	SphereComponent->SetSphereRadius(100.0f);
	SphereComponent->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	SphereComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AITItemActor::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AITItemActor, ItemInstance);
}

void AITItemActor::BeginPlay()
{
	Super::BeginPlay();

	GetWorld()->GetTimerManager().SetTimer(
		EnablePickupTimerHandle, this, &AITItemActor::EnablePickup, 0.1f, false);

	if (HasAuthority())
	{
		SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &AITItemActor::OnSphereOverlap);

		if (ItemInstance == nullptr && PlacedItemDefinition != nullptr)
		{
			UITItemInstance* NewItemInstance = NewObject<UITItemInstance>(this);
			NewItemInstance->InitItemInstance(PlacedItemDefinition, PlacedItemQuantity);
			InitItemActor(NewItemInstance);
		}
	}
}

void AITItemActor::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                   UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                   const FHitResult& SweepResult)
{
	if (HasAuthority() && ItemInstance)
	{
		if (AITCharacter* Player = Cast<AITCharacter>(OtherActor))
		{
			if (AITPlayerState* PlayerState = Player->GetITPlayerState())
			{
				if (ItemInstance->ItemDefinition->ItemTags.HasTag(ITItemGameplayTags::Item_Weapon))
				{
					if (UITWeaponManagerComponent* WeaponManagerComponent = PlayerState->GetITWeaponManagerComponent())
					{
						if (WeaponManagerComponent->GetMainWeaponInstance()
							&& WeaponManagerComponent->GetSubWeaponInstance())
						{
							return;
						}
						ItemInstance->Rename(nullptr, PlayerState);
						WeaponManagerComponent->ServerRPC_PickupWeapon(ItemInstance);
						Destroy();
					}
				}
				else if (ItemInstance->ItemDefinition->ItemTags.HasTag(ITItemGameplayTags::Item_Ammo))
				{
					if (UITAbilitySystemComponent* ASC = PlayerState->GetITAbilitySystemComponent())
					{
						if (UITItemDefinition* ItemDefinition = ItemInstance->GetItemDefinition())
						{
							float CurrentReserveAmmo = 0.0f;
							if (ItemDefinition->AmmoType == EAmmoType::NormalAmmo)
							{
								CurrentReserveAmmo = ASC->GetNumericAttribute(UITAmmoSet::GetNormalAmmoAttribute());
								ASC->SetNumericAttributeBase(
									UITAmmoSet::GetNormalAmmoAttribute(), CurrentReserveAmmo + PlacedItemQuantity);
								Destroy();
							}
							else if (ItemDefinition->AmmoType == EAmmoType::SpecialAmmo)
							{
								CurrentReserveAmmo = ASC->GetNumericAttribute(UITAmmoSet::GetSpecialAmmoAttribute());
								ASC->SetNumericAttributeBase(
									UITAmmoSet::GetSpecialAmmoAttribute(), CurrentReserveAmmo + PlacedItemQuantity);
								Destroy();
							}
						}
					}
				}
			}
		}
	}
}

UITItemInstance* AITItemActor::GetItemInstance() const
{
	return ItemInstance;
}

void AITItemActor::InitItemActor(UITItemInstance* InitItemInstance)
{
	if (HasAuthority())
	{
		ItemInstance = InitItemInstance;
		UpdateAppearance();
	}
}

void AITItemActor::UpdateAppearance()
{
	if (ItemInstance == nullptr || ItemInstance->ItemDefinition == nullptr)
	{
		return;
	}

	const UITItemDefinition* ItemDefinition = ItemInstance->ItemDefinition;
	if (ItemDefinition->ItemMesh)
	{
		StaticMeshComponent->SetStaticMesh(ItemDefinition->ItemMesh);
	}
	for (int32 i = 0; i < ItemDefinition->ItemMaterial.Num(); i++)
	{
		if (ItemDefinition->ItemMaterial[i])
		{
			StaticMeshComponent->SetMaterial(i, ItemDefinition->ItemMaterial[i]);
		}
	}
}

void AITItemActor::EnablePickup()
{
	SphereComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}
