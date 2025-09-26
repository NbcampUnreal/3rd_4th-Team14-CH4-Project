#pragma once

#include "Components/ActorComponent.h"
#include "GameplayTagContainer.h"
#include "NativeGameplayTags.h"
#include "Item/Weapon/ITWeaponManagerComponent.h"
#include "ITCharacterAnimComponent.generated.h"

namespace ITCameraModeTags
{
	IMITATIONTRIGGER_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(AnimLayer);
	IMITATIONTRIGGER_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(AnimLayer_UnArm);
	IMITATIONTRIGGER_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(AnimLayer_Pistol);
	IMITATIONTRIGGER_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(AnimLayer_Rifle);
}

class USkeletalMeshComponent;
class UAnimInstance;

USTRUCT(BlueprintType)
struct FITAnimLayerEntry
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UAnimInstance> AnimLayerClass = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Meta = (Categories = "AnimLayer"))
	FGameplayTag AnimLayerTag;
};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class IMITATIONTRIGGER_API UITCharacterAnimComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UITCharacterAnimComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	void SetAnimLayerRules(TArray<FITAnimLayerEntry> Rules);
	TSubclassOf<UAnimInstance> FindBestMatchAnimLayer();
	void UpdateAnimLayer();

	UFUNCTION()
	void OnUpdateCurrentWeapon(ECurrentWeaponSlot CurrentWeaponType);

protected:
	USkeletalMeshComponent* GetParentMeshComponent() const;

	TArray<FITAnimLayerEntry> AnimLayerRules;
	TSubclassOf<UAnimInstance> DefaultAnimLayer;
};
