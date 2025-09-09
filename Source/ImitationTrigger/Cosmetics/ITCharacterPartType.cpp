#include "Cosmetics/ITCharacterPartType.h"

FITCharacterPartHandle FITCharacterPartList::AddEntry(FITCharacterPart NewPart)
{
	return FITCharacterPartHandle();
}

void FITCharacterPartList::RemoveEntry(FITCharacterPartHandle Handle)
{
}

bool FITCharacterPartList::SpawnActorForEntry(FITAppliedCharacterPartEntry& Entry)
{
	return false;
}

void FITCharacterPartList::DestroyActorForEntry(FITAppliedCharacterPartEntry& Entry)
{
}

FGameplayTagContainer FITCharacterPartList::CollectCombinedTags() const
{
	return FGameplayTagContainer();
}
