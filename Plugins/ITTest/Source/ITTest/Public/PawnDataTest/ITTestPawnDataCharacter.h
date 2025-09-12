#pragma once

#include "AbilitySystemInterface.h"
#include "AbilitySystem/ITAbilitySet.h"
#include "Character/ITCharacter.h"
#include "ITTestPawnDataCharacter.generated.h"

class UITAbilitySystemComponent;

UCLASS()
class ITTEST_API AITTestPawnDataCharacter : public AITCharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AITTestPawnDataCharacter();
	virtual void BeginPlay() override;

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	UFUNCTION(BlueprintCallable)
	UITAbilitySystemComponent* GetITAbilitySystemComponent() const;

	UFUNCTION(BlueprintCallable, Category = "OnDebugKey")
	void OnDebugKeySkill(TSubclassOf<UGameplayAbility> AbilityClass);

	UFUNCTION(BlueprintCallable, Category = "OnDebugKey")
	void OnDebugKeyX();


protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UITAbilitySystemComponent> ASC;

private:
	FITAbilitySet_GrantedHandles GrantedHandles;
};
