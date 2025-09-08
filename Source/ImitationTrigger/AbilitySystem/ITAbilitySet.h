#pragma once

#include "ActiveGameplayEffectHandle.h"
#include "GameplayAbilitySpecHandle.h"
#include "AttributeSet.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "ITAbilitySet.generated.h"

class UGameplayAbility;
class UGameplayEffect;
class UAttributeSet;
class UITAbilitySystemComponent;


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
 * Pawn �Ǵ� Character���� �ο���(grant) Ability, Effects, Attributes�� �����ϱ� ���� ����ü
 * FLyraAbilitySet_GrantedHandles
 */
USTRUCT(BlueprintType)
struct FITAbilitySet_GrantedHandles
{
	GENERATED_BODY()

public:
	void AddAbilitySpecHandle(const FGameplayAbilitySpecHandle& Handle);
	void AddGameplayEffectHandle(const FActiveGameplayEffectHandle& Handle);
	void AddAttributeSet(UAttributeSet* Set);

	void TakeFromAbilitySystem(UITAbilitySystemComponent* ITASC);

protected:
	UPROPERTY()
	TArray<FGameplayAbilitySpecHandle> AbilitySpecHandles;

	UPROPERTY()
	TArray<FActiveGameplayEffectHandle> GameplayEffectHandles;

	UPROPERTY()
	TArray<TObjectPtr<UAttributeSet>> GrantedAttributeSets;
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
	UITAbilitySet(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	// DataAsset�� �����س��� Ability, Effect, Attribute�� ASC�� �����Ѵ�.
	void GiveToAbilitySystem(UITAbilitySystemComponent* ITASC, FITAbilitySet_GrantedHandles* OutGrantedHandles, UObject* SourceObject = nullptr) const;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Gameplay Abilities", meta = (TitleProperty = Ability))
	TArray<FITAbilitySet_GameplayAbility> GrantedGameplayAbilities;

	UPROPERTY(EditDefaultsOnly, Category = "Gameplay Effect", meta = (TitleProperty = GameplayEffect))
	TArray<FITAbilitySet_GameplayEffect> GrantedGameplayEffects;

	UPROPERTY(EditDefaultsOnly, Category = "Attribute Set", meta = (TitleProperty = AttributeSet))
	TArray<FITAbilitySet_AttributeSet> GrantedAttributes;

private:
	void GrantAttributeSets(UITAbilitySystemComponent* ITASC, FITAbilitySet_GrantedHandles* OutGrantedHandles) const;
	void GrantAbilities(UITAbilitySystemComponent* ITASC, FITAbilitySet_GrantedHandles* OutGrantedHandles, UObject* SourceObject = nullptr) const;
	void GrantGameplayEffects(UITAbilitySystemComponent* ITASC, FITAbilitySet_GrantedHandles* OutGrantedHandles) const;
};
