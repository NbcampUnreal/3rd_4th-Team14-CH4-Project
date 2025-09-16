// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "StructUtils/InstancedStruct.h"
#include "Item/ITItemFragment.h"
#include "ITItemDefinition.generated.h"

UENUM(BlueprintType)
enum class EItemPickupType : uint8
{
	Automatic, // 자동 획득
	Manual // 수동(상호작용) 후 획득
};

UCLASS(Blueprintable, BlueprintType, Const)
class IMITATIONTRIGGER_API UITItemDefinition : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item")
	FText ItemName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item")
	TSoftObjectPtr<UTexture2D> ItemIcon;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item", meta = (ClampMin = 1))
	int32 MaxStackSize = 1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item")
	EItemPickupType ItemPickupType;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item|Appearance")
	TSoftObjectPtr<UStaticMesh> ItemMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item|Appearance")
	TArray<TSoftObjectPtr<UMaterialInterface>> ItemMaterial;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item|Fragment",
		meta = (ShowInnerProperties, BaseStruct = "/Script/ImitationTrigger.ITItemFragment"))
	TArray<FInstancedStruct> Fragments;

public:
	UFUNCTION(BlueprintCallable, Category = "Item|Fragment")
	bool HasItemTag(FGameplayTag TagToFind) const;

	UFUNCTION(BlueprintCallable, Category = "Item|Fragment")
	FGameplayTagContainer GetItemTags() const;

	// C++용 프래그먼트 반환 함수
	template <typename T>
	const T* GetFragment() const;

#if WITH_EDITOR
	// 메시에 필요한 머티리얼 개수로 자동으로 변경해주는 에디터 함수
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
};

template <typename T>
const T* UITItemDefinition::GetFragment() const
{
	static_assert(TIsDerivedFrom<T, FITItemFragment>::IsDerived, "C++ 전용 템플릿 함수, 컴파일 안정성 보장");

	for (const FInstancedStruct& Fragment : Fragments)
	{
		if (const T* FoundFragment = Fragment.GetPtr<T>())
		{
			return FoundFragment;
		}
	}
	return nullptr;
}
