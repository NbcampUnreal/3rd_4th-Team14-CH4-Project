#pragma once

#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "ITAbilitySet.generated.h"

class UGameplayAbility;
class UGameplayEffect;
class UAttributeSet;
class UObject;


// Input Tag와 Ability를 Mapping하는 구조체
USTRUCT(BlueprintType)
struct FITAbilitySet_GameplayAbility
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGameplayAbility> Ability = nullptr;

	UPROPERTY(EditDefaultsOnly)
	int32 AbilityLevel = 1;

	UPROPERTY(EditDefaultsOnly, Meta = (Categories = "InputTag"))
	FGameplayTag InputTag;
};


// Pawn 또는 Character에게 기본으로 부여할 Effect (ex. passive effects)
USTRUCT(BlueprintType)
struct FITAbilitySet_GameplayEffect
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGameplayEffect> GameplayEffect = nullptr;

	UPROPERTY(EditDefaultsOnly)
	float EffectLevel = 1.0f;
};


// Pawn 또는 Character에게 추가로 부여할 Attribute Set
USTRUCT(BlueprintType)
struct FITAbilitySet_AttributeSet
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UAttributeSet> AttributeSet;
};


/**
 * Gamplay Ability, Effect, Attribute 정보의 모음
 * ULyraAbilitySet의 구성을 참고함
 */
UCLASS(BlueprintType, Const)
class IMITATIONTRIGGER_API UITAbilitySet : public UDataAsset
{
	GENERATED_BODY()

public:


protected:

	UPROPERTY(EditDefaultsOnly, Category = "Gameplay Abilities", meta = (TitleProperty = GameplayAbility))
	TArray<FITAbilitySet_GameplayAbility> GrantedGameplayAbilities;

	UPROPERTY(EditDefaultsOnly, Category = "Gameplay Effect", meta = (TitleProperty = GameplayEffect))
	TArray<FITAbilitySet_GameplayEffect> GrantedGameplayEffects;

	UPROPERTY(EditDefaultsOnly, Category = "Attribute Set", meta = (TitleProperty = AttributeSet))
	TArray<FITAbilitySet_AttributeSet> GrantedAttributes;
};
