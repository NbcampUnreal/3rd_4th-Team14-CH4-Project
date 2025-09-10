#pragma once

#include "GameplayTagContainer.h"
#include "Net/Serialization/FastArraySerializer.h"
#include "ITCharacterPartType.generated.h"

class UITCharacterPartComponent;

UENUM()
enum class ECharacterCustomizationCollisionMode : uint8
{
	NoCollision,
	UseCollisionFromCharacterPart
};


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
struct FITAppliedCharacterPartEntry : public FFastArraySerializerItem
{
	GENERATED_BODY()

	UPROPERTY()
	FITCharacterPart Part;

	UPROPERTY(NotReplicated)
	int32 PartHandle = INDEX_NONE;

	UPROPERTY(NotReplicated)
	TObjectPtr<UChildActorComponent> SpawnedComponent = nullptr;
};

USTRUCT(BlueprintType)
struct FITCharacterPartList : public FFastArraySerializer
{
	GENERATED_BODY()

	// FFastArraySerializer 관련 함수
	void PreReplicatedRemove(const TArrayView<int32> RemovedIndices, int32 FinalSize);
	void PostReplicatedAdd(const TArrayView<int32> AddedIndices, int32 FinalSize);
	void PostReplicatedChange(const TArrayView<int32> ChangedIndices, int32 FinalSize);
	bool NetDeltaSerialize(FNetDeltaSerializeInfo& DeltaParms)
	{
		return FFastArraySerializer::FastArrayDeltaSerialize<FITAppliedCharacterPartEntry, FITCharacterPartList>(Entries, DeltaParms, *this);
	}

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

	UPROPERTY(NotReplicated)
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

template<>
struct TStructOpsTypeTraits<FITCharacterPartList> : public TStructOpsTypeTraitsBase2<FITCharacterPartList>
{
	enum { WithNetDeltaSerializer = true };
};
