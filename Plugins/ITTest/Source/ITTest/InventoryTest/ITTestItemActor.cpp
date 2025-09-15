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
			FTimerHandle InitTimerHandle;
			GetWorld()->GetTimerManager().SetTimer(
				InitTimerHandle,
				[this]()
				{
					if (ItemInstance == nullptr && PlacedItemData != nullptr)
					{
						UITTestItemInstance* NewItemInstance = NewObject<UITTestItemInstance>(this);
						NewItemInstance->InitItemInstance(PlacedItemData, PlacedItemQuantity);
						InitializeItem(NewItemInstance);
					}
				},
				0.5f,
				false
			);
		}

		SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &AITTestItemActor::OnSphereOverlap);
	}
}

void AITTestItemActor::InitializeItem(UITTestItemInstance* InitItemInstance)
{
	if (HasAuthority())
	{
		ItemInstance = InitItemInstance;
		UpdateAppearance();

		if (StaticMeshComponent)
		{
			StaticMeshComponent->SetSimulatePhysics(true);
		}
	}
}

void AITTestItemActor::OnRep_ItemInstance(UITTestItemInstance* OldItemInstance)
{
	if (IsValid(OldItemInstance))
	{
		OldItemInstance->OnItemInstanceUpdated.RemoveDynamic(this, &AITTestItemActor::OnItemInstanceReady);
	}

	if (IsValid(ItemInstance))
	{
		if (IsValid(ItemInstance->ItemData))
		{
			UpdateAppearance();
		}
		else
		{
			ItemInstance->OnItemInstanceUpdated.AddUniqueDynamic(this, &AITTestItemActor::OnItemInstanceReady);
		}
	}
}

void AITTestItemActor::OnItemInstanceReady()
{
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
				InventoryComponent->ServerRPC_AddPickupItem(this);
			}
		}
	}
}

void AITTestItemActor::UpdateAppearance()
{
	if (!ItemInstance && !ItemInstance->ItemData)
	{
		return;
	}

	AsyncApplyMesh();
}

void AITTestItemActor::AsyncApplyMesh()
{
	if (ItemInstance->ItemData->ItemMesh.IsNull())
	{
		return;
	}

	FStreamableManager& Streamable = UAssetManager::Get().GetStreamableManager();
	Streamable.RequestAsyncLoad(ItemInstance->ItemData->ItemMesh.ToSoftObjectPath(), [this]()
	{
		if (IsValid(this) && ItemInstance && ItemInstance->ItemData && StaticMeshComponent)
		{
			if (UStaticMesh* LoadedMesh = ItemInstance->ItemData->ItemMesh.Get())
			{
				StaticMeshComponent->SetStaticMesh(LoadedMesh);
				AsyncApplyMaterials();
			}
		}
	});
}

void AITTestItemActor::AsyncApplyMaterials()
{
	if (ItemInstance->ItemData->ItemMaterial.Num() == 0)
	{
		return;
	}

	TArray<FSoftObjectPath> MaterialsToLoad;
	for (const auto& MaterialPtr : ItemInstance->ItemData->ItemMaterial)
	{
		if (!MaterialPtr.IsNull())
		{
			MaterialsToLoad.Add(MaterialPtr.ToSoftObjectPath());
		}
	}

	if (MaterialsToLoad.IsEmpty())
	{
		return;
	}

	FStreamableManager& Streamable = UAssetManager::Get().GetStreamableManager();
	Streamable.RequestAsyncLoad(MaterialsToLoad, [this]()
	{
		if (IsValid(this) && ItemInstance && ItemInstance->ItemData && StaticMeshComponent)
		{
			TWeakObjectPtr<AITTestItemActor> WeakThis(this);
			GetWorld()->GetTimerManager().SetTimerForNextTick([WeakThis]()
			{
				if (!WeakThis.IsValid() || !WeakThis->ItemInstance || !WeakThis->ItemInstance->ItemData) return;

				for (int32 i = 0; i < WeakThis->ItemInstance->ItemData->ItemMaterial.Num(); i++)
				{
					if (UMaterialInterface* LoadedMaterial = WeakThis->ItemInstance->ItemData->ItemMaterial[i].Get())
					{
						WeakThis->StaticMeshComponent->SetMaterial(i, LoadedMaterial);
					}
				}
			});
		}
	});
}
