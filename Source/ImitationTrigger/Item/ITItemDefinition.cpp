// Fill out your copyright notice in the Description page of Project Settings.


#include "ITItemDefinition.h"
#include "Fragment/ITItemFragment.h"
#include "Engine/StaticMesh.h"

UITItemDefinition::UITItemDefinition()
{
	for (UITItemFragment* Fragment : Fragments)
	{
		if (Fragment)
		{
			ItemTags.AppendTags(Fragment->FragmentTags);
		}
	}
}

UITItemFragment* UITItemDefinition::FindFragmentByClass(const TSubclassOf<UITItemFragment> FragmentClass) const
{
	UITItemFragment* FoundFragment = nullptr;

	if (UClass* TargetClass = FragmentClass.Get())
	{
		for (UITItemFragment* Fragment : Fragments)
		{
			if (Fragment && Fragment->IsA(TargetClass))
			{
				FoundFragment = Fragment;
				break;
			}
		}
	}
	return FoundFragment;
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
		if (ItemMesh)
		{
			const int32 MaterialSlots = ItemMesh->GetStaticMaterials().Num();
			ItemMaterial.SetNum(MaterialSlots);

			for (int32 i = 0; i < MaterialSlots; i++)
			{
				if (ItemMesh->GetStaticMaterials()[i].MaterialInterface)
				{
					ItemMaterial[i] = ItemMesh->GetStaticMaterials()[i].MaterialInterface;
				}
				else
				{
					ItemMaterial[i] = nullptr;
				}
			}
		}
		else
		{
			ItemMaterial.Empty();
		}
	}
}
#endif
