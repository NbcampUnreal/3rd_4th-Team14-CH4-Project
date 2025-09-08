#pragma once

#include "AbilitySystemInterface.h"
#include "GameFramework/PlayerState.h"
#include "ITPlayerState.generated.h"

class AITPlayerController;
class AITCharacter;
class UITAbilitySystemComponent;

UCLASS()
class IMITATIONTRIGGER_API AITPlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()
	
public:
	AITPlayerState(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UFUNCTION(BlueprintCallable, Category = "ITPlayerState")
	AITPlayerController* GetITPlayerController() const;

	UFUNCTION(BlueprintCallable, Category = "ITPlayerState")
	AITCharacter* GetITCharacter() const;

	UFUNCTION(BlueprintCallable, Category = "ITPlayerState")
	UITAbilitySystemComponent* GetLyraAbilitySystemComponent() const { return AbilitySystemComponent; }

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;


private:
	UPROPERTY(VisibleAnywhere, Category = "ITPlayerState|AbilitySystemComponent")
	TObjectPtr<UITAbilitySystemComponent> AbilitySystemComponent;
};
