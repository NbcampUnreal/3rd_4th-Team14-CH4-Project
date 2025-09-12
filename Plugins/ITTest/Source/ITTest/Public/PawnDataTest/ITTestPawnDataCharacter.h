#pragma once

#include "AbilitySystemInterface.h"
#include "AbilitySystem/ITAbilitySet.h"
#include "Character/ITCharacter.h"
#include "ITTestPawnDataCharacter.generated.h"

class UITAbilitySystemComponent;

UCLASS()
class ITTEST_API AITTestPawnDataCharacter : public AITCharacter
{
	GENERATED_BODY()

public:
	AITTestPawnDataCharacter();
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, Category = "OnDebugKey")
	void OnDebugKeySkill(TSubclassOf<UGameplayAbility> AbilityClass);

	UFUNCTION(BlueprintCallable, Category = "OnDebugKey")
	void OnDebugKeyX();


private:
	FITAbilitySet_GrantedHandles GrantedHandles;
};
