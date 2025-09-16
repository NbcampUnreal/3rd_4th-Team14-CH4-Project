// Fill out your copyright notice in the Description page of Project Settings.


#include "ITItemDefinition.h"
#include "Engine/StaticMesh.h"

bool UITItemDefinition::FindFragmentInBlueprint(
	const UScriptStruct* FragmentStructType, FInstancedStruct& OutFragment) const
{
	if (!FragmentStructType)
	{
		return false;
	}
	if (!FragmentStructType->IsChildOf(FITItemFragment::StaticStruct()))
	{
		return false;
	}

	for (const FInstancedStruct& Fragment : Fragments)
	{
		if (Fragment.GetScriptStruct() == FragmentStructType)
		{
			OutFragment = Fragment;
			return true;
		}
	}
	return false;
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
