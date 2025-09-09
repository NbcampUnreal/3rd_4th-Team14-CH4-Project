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
};

USTRUCT()
struct FITAppliedCharacterPartEntry
{
	GENERATED_BODY()
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
	void DestroyActorForEntry(FITAppliedCharacterPartEntry& Entry);

	FGameplayTagContainer CollectCombinedTags() const;


	UPROPERTY()
	TArray<FITAppliedCharacterPartEntry> Entries;

	UPROPERTY()
	TObjectPtr<UITCharacterPartComponent> OwnerComponent;

	int32 PartHandleCounter = 0;
};




USTRUCT()
struct FITAnimBodyStyleSelectionSet
{
	GENERATED_BODY()
};
