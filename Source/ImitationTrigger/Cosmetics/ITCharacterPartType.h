#pragma once

#include "GameplayTagContainer.h"
#include "ITCharacterPartType.generated.h"

class UITCharacterPartComponent;

USTRUCT(BlueprintType)
struct FITCharacterPart
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AActor> PartClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName SocketName;
};

USTRUCT(BlueprintType)
struct FITCharacterPartHandle
{
	GENERATED_BODY()

	void Reset()
	{
		PartHandle = INDEX_NONE;
	}

	bool IsValid() const
	{
		return PartHandle != INDEX_NONE;
	}

	UPROPERTY()
	int32 PartHandle = INDEX_NONE;
};

USTRUCT()
struct FITAppliedCharacterPartEntry
{
	GENERATED_BODY()

	UPROPERTY()
	FITCharacterPart Part;

	UPROPERTY()
	int32 PartHandle = INDEX_NONE;

	UPROPERTY()
	TObjectPtr<UChildActorComponent> SpawnedComponent = nullptr;
};

USTRUCT(BlueprintType)
struct FITCharacterPartList
{
	GENERATED_BODY()

	FITCharacterPartList()
	{
		OwnerComponent = nullptr;
	}
	FITCharacterPartList(UITCharacterPartComponent* InOwnerComponent)
	{
		OwnerComponent = InOwnerComponent;
	}

	FITCharacterPartHandle AddEntry(FITCharacterPart NewPart);
	void RemoveEntry(FITCharacterPartHandle Handle);

	bool SpawnActorForEntry(FITAppliedCharacterPartEntry& Entry);
	bool DestroyActorForEntry(FITAppliedCharacterPartEntry& Entry);

	FGameplayTagContainer CollectCombinedTags() const;


	UPROPERTY()
	TArray<FITAppliedCharacterPartEntry> Entries;

	UPROPERTY()
	TObjectPtr<UITCharacterPartComponent> OwnerComponent;

	int32 PartHandleCounter = 0;
};

USTRUCT(BlueprintType)
struct FITAnimBodyStyleSelectionEntry
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<USkeletalMesh> Mesh = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTagContainer RequiredTags;
};

USTRUCT(BlueprintType)
struct FITAnimBodyStyleSelectionSet
{
	GENERATED_BODY()

	USkeletalMesh* SelectBestBodyStyle(const FGameplayTagContainer& CosmeticTags) const;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FITAnimBodyStyleSelectionEntry> MeshRules;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<USkeletalMesh> DefaultMesh = nullptr;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UPhysicsAsset> ForcedPhysicsAsset = nullptr;
};
