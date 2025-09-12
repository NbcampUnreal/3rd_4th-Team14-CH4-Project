#include "Cosmetics/ITCharacterPartType.h"
#include "Cosmetics/ITCharacterPartComponent.h"
#include "GameplayTagAssetInterface.h"

void FITCharacterPartList::PreReplicatedRemove(const TArrayView<int32> RemovedIndices, int32 FinalSize)
{
	bool bIsChanged = false;
	for (int32 Index : RemovedIndices)
	{
		FITAppliedCharacterPartEntry& PartEntry = Entries[Index];
		bIsChanged |= DestroyActorForEntry(PartEntry);
	}
	if (bIsChanged && IsValid(OwnerComponent))
	{
		OwnerComponent->BroadcastChanged();
	}
}

void FITCharacterPartList::PostReplicatedAdd(const TArrayView<int32> AddedIndices, int32 FinalSize)
{
	bool bIsChanged = false;
	for (int32 Index : AddedIndices)
	{
		FITAppliedCharacterPartEntry& PartEntry = Entries[Index];
		bIsChanged |= SpawnActorForEntry(PartEntry);
	}
	if (bIsChanged && IsValid(OwnerComponent))
	{
		OwnerComponent->BroadcastChanged();
	}
}

void FITCharacterPartList::PostReplicatedChange(const TArrayView<int32> ChangedIndices, int32 FinalSize)
{
	bool bIsChanged = false;
	for (int32 Index : ChangedIndices)
	{
		FITAppliedCharacterPartEntry& PartEntry = Entries[Index];
		bIsChanged |= DestroyActorForEntry(PartEntry);
		bIsChanged |= SpawnActorForEntry(PartEntry);
	}
	if (bIsChanged && IsValid(OwnerComponent))
	{
		OwnerComponent->BroadcastChanged();
	}
}

FITCharacterPartHandle FITCharacterPartList::AddEntry(FITCharacterPart NewPart)
{
	FITCharacterPartHandle Result;
	Result.PartHandle = PartHandleCounter++;

	if (IsValid(OwnerComponent))
	{
		AActor* OwnerActor = OwnerComponent->GetOwner();
		if (IsValid(OwnerActor) && OwnerActor->HasAuthority())
		{
			FITAppliedCharacterPartEntry& NewEntry = Entries.AddDefaulted_GetRef();
			NewEntry.PartHandle = Result.PartHandle;
			NewEntry.Part = NewPart;

			bool SpawnResult = SpawnActorForEntry(NewEntry);
			if (SpawnResult)
			{
				OwnerComponent->BroadcastChanged();
			}
			MarkItemDirty(NewEntry);
		}
	}
	return Result;
}

void FITCharacterPartList::RemoveEntry(FITCharacterPartHandle Handle)
{
	for (auto EntryIterator = Entries.CreateIterator(); EntryIterator; ++EntryIterator)
	{
		FITAppliedCharacterPartEntry& Entry = *EntryIterator;
		if (Entry.PartHandle == Handle.PartHandle)
		{
			bool DestroyResult = DestroyActorForEntry(Entry);
			EntryIterator.RemoveCurrent();
			MarkArrayDirty();

			if (IsValid(OwnerComponent) && DestroyResult)
			{
				OwnerComponent->BroadcastChanged();
			}
			break;
		}
	}
}

bool FITCharacterPartList::SpawnActorForEntry(FITAppliedCharacterPartEntry& Entry)
{
	bool bCreatedAnyActor = false;

	if (IsValid(OwnerComponent))
	{
		AActor* OwnerActor = OwnerComponent->GetOwner();
		bool bIsNotDedicatedServer = !OwnerComponent->IsNetMode(NM_DedicatedServer);
		if (IsValid(OwnerActor) && bIsNotDedicatedServer)
		{
			if (Entry.Part.PartClass != nullptr)
			{
				if (USceneComponent* ComponentToAttachTo = OwnerComponent->GetSceneComponentToAttachTo())
				{
					const FTransform SpawnTransform = ComponentToAttachTo->GetSocketTransform(Entry.Part.SocketName);

					AActor* PartOuter = OwnerComponent->GetOwner();
					UChildActorComponent* PartComponent = NewObject<UChildActorComponent>(PartOuter);
					PartComponent->SetupAttachment(ComponentToAttachTo, Entry.Part.SocketName);
					PartComponent->SetChildActorClass(Entry.Part.PartClass);
					PartComponent->RegisterComponent();

					if (AActor* SpawnedActor = PartComponent->GetChildActor())
					{
						if (USceneComponent* SpawnedRootComponent = SpawnedActor->GetRootComponent())
						{
							// 스폰된 Actor가 parent보다 먼저 Tick()하지 않도록, 선행 조건을 붙인다.
							SpawnedRootComponent->AddTickPrerequisiteComponent(ComponentToAttachTo);
						}
					}
					Entry.SpawnedComponent = PartComponent;
					bCreatedAnyActor = true;
				}
			}
		}
	}
	return bCreatedAnyActor;
}

bool FITCharacterPartList::DestroyActorForEntry(FITAppliedCharacterPartEntry& Entry)
{
	bool bDestroyedAnyActor = false;

	if (Entry.SpawnedComponent != nullptr)
	{
		Entry.SpawnedComponent->DestroyComponent();
		Entry.SpawnedComponent = nullptr;
		bDestroyedAnyActor = true;
	}
	return bDestroyedAnyActor;
}

FGameplayTagContainer FITCharacterPartList::CollectCombinedTags() const
{
	FGameplayTagContainer Result;

	for (const FITAppliedCharacterPartEntry& Entry : Entries)
	{
		if (Entry.SpawnedComponent)
		{
			// TaggedActor가 IGameplayTagAssetInterface를 상속 받으면, Tag 관련 기능을 제공할 수 있다.
			// GetOwnedGameplayTags() 함수는 직접 오버라이딩 해야 한다.
			AActor* ChildActor = Entry.SpawnedComponent->GetChildActor();
			if (IGameplayTagAssetInterface* TagInterface = Cast<IGameplayTagAssetInterface>(ChildActor))
			{
				TagInterface->GetOwnedGameplayTags(Result);
			}
		}
	}
	return Result;
}

USkeletalMesh* FITAnimBodyStyleSelectionSet::SelectBestBodyStyle(const FGameplayTagContainer& CosmeticTags) const
{
	for (const FITAnimBodyStyleSelectionEntry& MeshRule : MeshRules)
	{
		if ((MeshRule.Mesh) && (CosmeticTags.HasAll(MeshRule.RequiredTags)))
		{
			return MeshRule.Mesh;
		}
	}
	return DefaultMesh;
}
