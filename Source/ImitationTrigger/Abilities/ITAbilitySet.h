#pragma once

#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "ITAbilitySet.generated.h"

class UGameplayAbility;
class UGameplayEffect;
class UAttributeSet;
class UObject;


// Input Tag�� Ability�� Mapping�ϴ� ����ü
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


// Pawn �Ǵ� Character���� �⺻���� �ο��� Effect (ex. passive effects)
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


// Pawn �Ǵ� Character���� �߰��� �ο��� Attribute Set
USTRUCT(BlueprintType)
struct FITAbilitySet_AttributeSet
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UAttributeSet> AttributeSet;
};


/**
 * Gamplay Ability, Effect, Attribute ������ ����
 * ULyraAbilitySet�� ������ ������
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
