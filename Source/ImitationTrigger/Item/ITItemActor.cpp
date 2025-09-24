// Fill out your copyright notice in the Description page of Project Settings.


#include "ITItemActor.h"
#include "ITItemDefinition.h"
#include "ITItemInstance.h"
#include "Components/SphereComponent.h"
#include "Net/UnrealNetwork.h"

AITItemActor::AITItemActor()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
	SetReplicateMovement(true);

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	SetRootComponent(StaticMeshComponent);
	StaticMeshComponent->SetIsReplicated(true);

	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	SphereComponent->SetupAttachment(RootComponent);
	SphereComponent->SetSphereRadius(100.0f);
	SphereComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SphereComponent->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
}

void AITItemActor::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AITItemActor, ItemInstance);
}

void AITItemActor::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		if (ItemInstance == nullptr && PlacedItemDefinition != nullptr)
		{
			UITItemInstance* NewItemInstance = NewObject<UITItemInstance>(this);
			NewItemInstance->InitItemInstance(PlacedItemDefinition, PlacedItemQuantity);
			InitItemActor(NewItemInstance);
		}
	}

	StaticMeshComponent->SetSimulatePhysics(true);
	StaticMeshComponent->SetCollisionProfileName(TEXT("PhysicsActor"));
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

void AITItemActor::OnRep_ItemInstance()
{
	UpdateAppearance();
}
