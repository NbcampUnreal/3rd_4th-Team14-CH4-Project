#pragma once

#include "Components/ActorComponent.h"
#include "Cosmetics/ITCharacterPartType.h"
#include "GameplayTagContainer.h"
#include "ITCharacterPartComponent.generated.h"

class USkeletalMeshComponent;
class USceneComponent;
class UITPawnData;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class IMITATIONTRIGGER_API UITCharacterPartComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UITCharacterPartComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	void SetBodyMeshes(const FITAnimBodyStyleSelectionSet InBodyMeshes);

	void AddInitCharacterParts(const TArray<FITCharacterPart>& InitCharacterParts);
	void ClearCharacterParts();

	FITCharacterPartHandle AddCharacterPart(const FITCharacterPart& NewPart);
	void RemoveCharacterPart(FITCharacterPartHandle Handle);
	

	USkeletalMeshComponent* GetParentMeshComponent() const;
	USceneComponent* GetSceneComponentToAttachTo() const;

	void BroadcastChanged();

	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = Costmetic)
	FGameplayTagContainer GetCombinedTags(FGameplayTag RequiredPrefix) const;

	UPROPERTY(Replicated, Transient)
	FITCharacterPartList AppliedCharacterPartList;

	FITAnimBodyStyleSelectionSet BodyMeshes;
};
