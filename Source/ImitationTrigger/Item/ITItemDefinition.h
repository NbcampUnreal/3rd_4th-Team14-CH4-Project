// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "ITItemDefinition.generated.h"

class UITItemFragment;

UENUM(BlueprintType)
enum class EAmmoType : uint8
{
	NormalAmmo,
	SpecialAmmo
};

UCLASS(Blueprintable, BlueprintType, Const)
class IMITATIONTRIGGER_API UITItemDefinition : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item")
	FGameplayTagContainer ItemTags;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item")
	FText ItemName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item")
	TObjectPtr<UTexture2D> ItemIcon;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item", meta = (ClampMin = 1))
	int32 MaxStackSize = 1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item|Appearance")
	TObjectPtr<UStaticMesh> ItemMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item|Appearance")
	TArray<TObjectPtr<UMaterialInterface>> ItemMaterial;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	EAmmoType AmmoType = EAmmoType::NormalAmmo;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Instanced, Category = "Item|Fragment")
	TArray<TObjectPtr<UITItemFragment>> Fragments;

public:
	UITItemDefinition();

	UFUNCTION(BlueprintCallable, Category = "Item|Fragment")
	UITItemFragment* FindFragmentByClass(const TSubclassOf<UITItemFragment> FragmentClass) const;

#if WITH_EDITOR
	// 메시에 필요한 머티리얼 개수로 자동으로 변경해주는 에디터 함수
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
};
