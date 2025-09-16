// Fill out your copyright notice in the Description page of Project Settings.


#include "ITItemDefinition.h"
#include "Engine/StaticMesh.h"

bool UITItemDefinition::HasItemTag(FGameplayTag TagToFind) const
{
	if (!TagToFind.IsValid())
	{
		return false;
	}

	for (const FInstancedStruct& Fragment : Fragments)
	{
		if (const FITItemFragment* BaseFragment = Fragment.GetPtr<FITItemFragment>())
		{
			if (BaseFragment->FragmentTag == TagToFind)
			{
				return true;
			}
		}
	}
	return false;
}

FGameplayTagContainer UITItemDefinition::GetItemTags() const
{
	FGameplayTagContainer FoundTags;

	for (const FInstancedStruct& Fragment : Fragments)
	{
		if (const FITItemFragment* BaseFragment = Fragment.GetPtr<FITItemFragment>())
		{
			if (BaseFragment->FragmentTag.IsValid())
			{
				FoundTags.AddTag(BaseFragment->FragmentTag);
			}
		}
	}
	return FoundTags;
}


#if WITH_EDITOR
void UITItemDefinition::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	FName PropertyName = NAME_None;
	if (PropertyChangedEvent.Property != nullptr)
	{
		PropertyName = PropertyChangedEvent.Property->GetFName();
	}

	if (PropertyName == GET_MEMBER_NAME_CHECKED(UITItemDefinition, ItemMesh))
	{
		if (!ItemMesh.IsNull())
		{
			if (UStaticMesh* Mesh = ItemMesh.LoadSynchronous())
			{
				const int32 MaterialSlots = Mesh->GetStaticMaterials().Num();
				ItemMaterial.SetNum(MaterialSlots);
			}
		}
		else
		{
			ItemMaterial.Empty();
		}
	}
}
#endif
