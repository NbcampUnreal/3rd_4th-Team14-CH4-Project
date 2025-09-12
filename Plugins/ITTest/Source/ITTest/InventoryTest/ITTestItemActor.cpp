// Fill out your copyright notice in the Description page of Project Settings.


#include "ITTestItemActor.h"
#include "ITTestInventoryComponent.h"
#include "ITTestItemData.h"
#include "ITTestItemInstance.h"
#include "Components/SphereComponent.h"
#include "Engine/AssetManager.h"
#include "Engine/StreamableManager.h"
#include "Net/UnrealNetwork.h"

AITTestItemActor::AITTestItemActor()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
	SetReplicateMovement(true);

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	SetRootComponent(StaticMeshComponent);
	StaticMeshComponent->SetIsReplicated(true);
	StaticMeshComponent->SetCollisionProfileName(TEXT("PhysicsActor"));

	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	SphereComponent->SetupAttachment(RootComponent);
	SphereComponent->SetSphereRadius(100.0f);
	SphereComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SphereComponent->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
}

void AITTestItemActor::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AITTestItemActor, ItemInstance);
}

void AITTestItemActor::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		if (ItemInstance == nullptr && PlacedItemData != nullptr)
		{
			UITTestItemInstance* NewItemInstance = NewObject<UITTestItemInstance>(this);
			NewItemInstance->InitItemInstance(PlacedItemData, PlacedItemQuantity);

			InitializeItem(NewItemInstance);
		}

		SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &AITTestItemActor::OnSphereOverlap);
	}
}

void AITTestItemActor::InitializeItem(UITTestItemInstance* InitItemInstance)
{
	if (HasAuthority())
	{
		ItemInstance = InitItemInstance;
		OnRep_ItemInstance();

		if (StaticMeshComponent)
		{
			StaticMeshComponent->SetSimulatePhysics(true);
		}
	}
}

void AITTestItemActor::OnRep_ItemInstance()
{
	if (ItemInstance)
	{
		ItemInstance->OnItemInstanceUpdated.AddUniqueDynamic(this, &AITTestItemActor::UpdateAppearance);
	}

	UpdateAppearance();
}

void AITTestItemActor::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                       UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep,
                                       const FHitResult& SweepResult)
{
	if (HasAuthority())
	{
		if (UITTestInventoryComponent* InventoryComponent
			= OtherActor->FindComponentByClass<UITTestInventoryComponent>())
		{
			if (ItemInstance)
			{
				InventoryComponent->Server_AddItem(ItemInstance->ItemData, ItemInstance->Quantity);
				Destroy();
			}
		}
	}
}

void AITTestItemActor::UpdateAppearance()
{
	if (ItemInstance && ItemInstance->ItemData)
	{
		if (ItemInstance->ItemData->ItemMesh.IsNull())
		{
			return;
		}

		FStreamableManager& Streamable = UAssetManager::GetStreamableManager();
		Streamable.RequestAsyncLoad(ItemInstance->ItemData->ItemMesh.ToSoftObjectPath(), [this]()
		{
			if (ItemInstance && ItemInstance->ItemData && StaticMeshComponent)
			{
				if (UStaticMesh* LoadedMesh = ItemInstance->ItemData->ItemMesh.Get())
				{
					StaticMeshComponent->SetStaticMesh(LoadedMesh);
				}
			}
		});

		if (ItemInstance->ItemData->ItemMaterial.Num() > 0)
		{
			for (int32 i = 0; i < ItemInstance->ItemData->ItemMaterial.Num(); ++i)
			{
				if (!ItemInstance->ItemData->ItemMaterial[i].IsNull())
				{
					Streamable.RequestAsyncLoad(ItemInstance->ItemData->ItemMaterial[i].ToSoftObjectPath(), [this, i]()
					{
						if (ItemInstance && ItemInstance->ItemData && StaticMeshComponent)
						{
							if (UMaterialInterface* LoadedMaterial = ItemInstance->ItemData->ItemMaterial[i].Get())
							{
								StaticMeshComponent->SetMaterial(i, LoadedMaterial);
							}
						}
					});
				}
			}
		}
	}
}
