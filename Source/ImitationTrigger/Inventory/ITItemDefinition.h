// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "StructUtils/InstancedStruct.h"
#include "Inventory/ITItemFragment.h"
#include "ITItemDefinition.generated.h"

UCLASS()
class IMITATIONTRIGGER_API UITItemDefinition : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item")
	FGameplayTag ItemTag;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item")
	FText ItemName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item")
	TSoftObjectPtr<UTexture2D> ItemIcon;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item", meta = (ClampMin = 1))
	int32 MaxStackSize = 1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item|Appearance")
	TSoftObjectPtr<UStaticMesh> ItemMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item|Appearance")
	TArray<TSoftObjectPtr<UMaterialInterface>> ItemMaterial;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item|Fragment",
		meta = (ShowInnerProperties, BaseStruct = "/Script/ImitationTrigger.ITItemFragment"))
	TArray<FInstancedStruct> Fragments;

	/* 특정 Fragment를 찾아주는 헬퍼 함수
	* GetPtr(): 못 찾을 경우, nullptr 반환하는 점을 이용해 아래와 같이 사용
	* if (FITEquipableFragment* EquipFragment = ItemDef->FindFragment<FITEquipableFragment>())
	*/
	template <typename T>
	const T* FindFragment() const;

#if WITH_EDITOR
	// 메시에 필요한 머티리얼 개수로 자동으로 변경해주는 에디터 함수
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
};

template <typename T>
const T* UITItemDefinition::FindFragment() const
{
	static_assert(TIsDerivedFrom<T, FITItemFragment>::IsDerived, "상속받은 구조체인지 컴파일 타임에 확인");
	for (const FInstancedStruct& Fragment : Fragments)
	{
		if (const T* FoundFragment = Fragment.GetPtr<T>())
		{
			return FoundFragment;
		}
	}
	return nullptr;
}
