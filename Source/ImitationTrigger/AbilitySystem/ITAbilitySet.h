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
 * Pawn 또는 Character에게 부여한(grant) Ability, Effects, Attributes를 관리하기 위한 구조체
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
 * Gamplay Ability, Effect, Attribute 정보의 모음
 * ULyraAbilitySet의 구성을 참고함
 */
UCLASS(BlueprintType, Const)
class IMITATIONTRIGGER_API UITAbilitySet : public UDataAsset
{
	GENERATED_BODY()

public:
	UITAbilitySet(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	// DataAsset에 설정해놓은 Ability, Effect, Attribute를 ASC에 적용한다.
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
