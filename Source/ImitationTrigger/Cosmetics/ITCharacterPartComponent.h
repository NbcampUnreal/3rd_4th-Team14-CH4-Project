#pragma once

#include "Components/ActorComponent.h"
#include "Cosmetics/ITCharacterPartType.h"
#include "GameplayTagContainer.h"
#include "ITCharacterPartComponent.generated.h"

class USkeletalMeshComponent;
class USceneComponent;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent), Blueprintable, BlueprintType)
class UITCharacterPartComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UITCharacterPartComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	FITCharacterPartHandle AddCharacterPart(const FITCharacterPart& NewPart);
	void RemoveCharacterPart(FITCharacterPartHandle Handle);

	USkeletalMeshComponent* GetParentMeshComponent() const;
	USceneComponent* GetSceneComponentToAttachTo() const;

	void BroadcastChanged();

	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = Costmetic)
	FGameplayTagContainer GetCombinedTags(FGameplayTag RequiredPrefix) const;

	UPROPERTY(Replicated, Transient)
	FITCharacterPartList AppliedCharacterPartList;

	UPROPERTY(EditAnywhere, Category = Cosmetics)
	FITAnimBodyStyleSelectionSet BodyMeshes;

	UPROPERTY(EditAnywhere, Category = Cosmetics)
	TArray<FITCharacterPart> CharacterParts;
};
