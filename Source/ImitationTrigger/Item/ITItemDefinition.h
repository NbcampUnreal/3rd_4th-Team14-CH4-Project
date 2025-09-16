// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "StructUtils/InstancedStruct.h"
#include "Item/ITItemFragment.h"
#include "ITItemDefinition.generated.h"

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

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item|Appearance")
	TSoftObjectPtr<UStaticMesh> ItemMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item|Appearance")
	TArray<TSoftObjectPtr<UMaterialInterface>> ItemMaterial;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item|Fragment",
		meta = (ShowInnerProperties, BaseStruct = "/Script/ImitationTrigger.ITItemFragment"))
	TArray<FInstancedStruct> Fragments;

public:
	UFUNCTION(BlueprintCallable, Category = "Item|Fragment")
	TArray<FInstancedStruct> FindFragmentByTag(FGameplayTag Tag) const;

	UFUNCTION(BlueprintCallable, Category = "Item|Fragment")
	bool HasItemTag(FGameplayTag TagToFind) const;

	UFUNCTION(BlueprintCallable, Category = "Item|Fragment")
	FGameplayTagContainer GetItemTags() const;

#if WITH_EDITOR
	// 메시에 필요한 머티리얼 개수로 자동으로 변경해주는 에디터 함수
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
};
