// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ITSpawnItemManagerActor.generated.h"

UCLASS()
class IMITATIONTRIGGER_API AITSpawnItemManagerActor : public AActor
{
	GENERATED_BODY()

public:
	AITSpawnItemManagerActor();

	// 게임 시작 시 아이템 배치 // 타이밍(BeginPlay 순서) 고려 필수
	UFUNCTION(BlueprintCallable, Category="IT|SpawnItem")
	void SpawnInitialItems();

	// 보급품? 스폰
	UFUNCTION(BlueprintCallable, Category="IT|SpawnItem")
	void SpawnSpecialItems(FVector LocationToSpawn, int32 NumToSpawn);

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "IT|SpawnItem")
	TObjectPtr<UDataTable> InitialItemDataTable;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "IT|SpawnItem")
	TObjectPtr<UDataTable> SpecialItemDataTable;

	// 스폰 포인트로 사용될 타겟포인트 액터 클래스
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "IT|Initial Spawn")
	TSubclassOf<AActor> InitialSpawnPointClass;

	// 반경 내에서 분산 스폰
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "IT|SpawnItem")
	float SpawnRadius = 500.0f;

	// 스폰 포인트마다 소환될 아이템 수
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "IT|SpawnItem")
	int32 NumInitialItemsToSpawn = 0;

private:
	void SpawnItem(FVector LocationToSpawn, UDataTable* ItemDataTable, int32 NumToSpawn);

	TArray<FVector> InitialSpawnLocations;
};
