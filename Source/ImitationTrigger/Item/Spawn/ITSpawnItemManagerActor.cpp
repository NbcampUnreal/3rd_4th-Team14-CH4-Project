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
		// 스폰 포인트가 너무 많을 경우, 타이머 추가
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
	ItemDataTable->GetAllRows<FITSpawnItemData>(TEXT(""), SpawnCandidates);
	float TotalWeight = 0.0f;

	for (int32 i = SpawnCandidates.Num() - 1; i >= 0; i--)
	{
		FITSpawnItemData* ItemData = SpawnCandidates[i];
		if (ItemData && ItemData->ItemActorClass)
		{
			TotalWeight += ItemData->Weight;
		}
		else
		{
			SpawnCandidates.RemoveAtSwap(i);
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
			FVector InitialSpawnLocation = LocationToSpawn;
			if (NumToSpawn > 1)
			{
				const float Angle = (360.0f / NumToSpawn) * i;
				InitialSpawnLocation += FRotator(0.f, Angle, 0.f).Vector() * SpawnRadius;
			}

			FVector FinalSpawnLocation = InitialSpawnLocation;
			FHitResult HitResult;
			FVector TraceStart = InitialSpawnLocation + FVector(0.f, 0.f, 2000.f);
			FVector TraceEnd = InitialSpawnLocation - FVector(0.f, 0.f, 2000.f);

			FCollisionQueryParams CollisionQueryParams;
			CollisionQueryParams.AddIgnoredActor(this);
			if (GetWorld()->LineTraceSingleByChannel(
				HitResult, TraceStart, TraceEnd, ECC_Visibility, CollisionQueryParams))
			{
				FinalSpawnLocation = HitResult.ImpactPoint + FVector(0.f, 0.f, 50.f);
			}

			const FRotator SpawnRotation = FRotator(0.f, FMath::FRandRange(0.f, 360.f), 0.f);
			const FTransform SpawnTransform(SpawnRotation, FinalSpawnLocation, FVector::OneVector);

			FActorSpawnParameters SpawnParams;
			SpawnParams.SpawnCollisionHandlingOverride
				= ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
			GetWorld()->SpawnActor<AITItemActor>(ItemDataToSpawn->ItemActorClass, SpawnTransform, SpawnParams);
		}
	}
}
