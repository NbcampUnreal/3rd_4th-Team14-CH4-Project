// Fill out your copyright notice in the Description page of Project Settings.


#include "ITTestItemData.h"

#if WITH_EDITOR
#include "Engine/StaticMesh.h"

void UITTestItemData::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	FName PropertyName = NAME_None;
	if (PropertyChangedEvent.Property != nullptr)
	{
		PropertyName = PropertyChangedEvent.Property->GetFName();
	}

	if (PropertyName == GET_MEMBER_NAME_CHECKED(UITTestItemData, ItemMesh))
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
