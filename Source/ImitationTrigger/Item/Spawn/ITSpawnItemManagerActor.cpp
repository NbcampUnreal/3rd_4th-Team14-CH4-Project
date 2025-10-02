// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/Spawn/ITSpawnItemManagerActor.h"
#include "ITSpawnItemData.h"
#include "Kismet/GameplayStatics.h"

AITSpawnItemManagerActor::AITSpawnItemManagerActor()
{
	PrimaryActorTick.bCanEverTick = false;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));

	bReplicates = true;
}

void AITSpawnItemManagerActor::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority() && InitialSpawnPointClass)
	{
		TArray<AActor*> FoundActors;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), InitialSpawnPointClass, FoundActors);

		for (AActor* Actor : FoundActors)
		{
			InitialSpawnLocations.Add(Actor->GetActorLocation());
		}
	}
}

void AITSpawnItemManagerActor::SpawnInitialItems()
{
	if (HasAuthority())
	{
		for (FVector SpawnLocation : InitialSpawnLocations)
		{
			SpawnItem(SpawnLocation, InitialItemDataTable, NumInitialItemsToSpawn);
		}
	}
}

void AITSpawnItemManagerActor::SpawnSpecialItems(FVector LocationToSpawn, int32 NumToSpawn)
{
	if (HasAuthority())
	{
		SpawnItem(LocationToSpawn, SpecialItemDataTable, NumToSpawn);
	}
}

void AITSpawnItemManagerActor::SpawnItem(FVector LocationToSpawn, UDataTable* ItemDataTable, int32 NumToSpawn)
{
	if (NumToSpawn <= 0 || !ItemDataTable)
	{
		return;
	}

	TArray<FITSpawnItemData*> SpawnCandidates;
	float TotalWeight = 0.0f;

	for (const TPair<FName, uint8*>& Row : ItemDataTable->GetRowMap())
	{
		FITSpawnItemData* ItemData = reinterpret_cast<FITSpawnItemData*>(Row.Value);
		if (ItemData && ItemData->ItemActorClass)
		{
			SpawnCandidates.Add(ItemData);
			TotalWeight += ItemData->Weight;
		}
	}

	if (TotalWeight <= 0.0f || SpawnCandidates.IsEmpty())
	{
		return;
	}

	for (int32 i = 0; i < NumToSpawn; i++)
	{
		float RandomWeight = FMath::FRandRange(0.0f, TotalWeight);
		float CurrentWeight = 0.0f;
		FITSpawnItemData* ItemDataToSpawn = SpawnCandidates.Last();

		for (FITSpawnItemData* SpawnCandidate : SpawnCandidates)
		{
			CurrentWeight += SpawnCandidate->Weight;
			if (RandomWeight <= CurrentWeight)
			{
				ItemDataToSpawn = SpawnCandidate;
				break;
			}
		}

		if (ItemDataToSpawn)
		{
			FVector SpawnLocation = LocationToSpawn;
			if (NumToSpawn > 1)
			{
				const float Angle = (360.0f / NumToSpawn) * i;
				SpawnLocation += FRotator(0.f, Angle, 0.f).Vector() * SpawnRadius;
			}
			const FRotator SpawnRotation = FRotator(0.f, FMath::FRandRange(0.f, 360.f), 0.f);
			const FTransform SpawnTransform(SpawnRotation, SpawnLocation, FVector::OneVector);

			FActorSpawnParameters SpawnParams;
			SpawnParams.SpawnCollisionHandlingOverride
				= ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
			GetWorld()->SpawnActor<AITItemActor>(ItemDataToSpawn->ItemActorClass, SpawnTransform, SpawnParams);
		}
	}
}
