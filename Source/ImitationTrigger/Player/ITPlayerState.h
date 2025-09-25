#pragma once

#include "AbilitySystemInterface.h"
#include "AbilitySystem/ITAbilitySet.h"
#include "GameFramework/PlayerState.h"
#include "ITPlayerState.generated.h"

class AITPlayerController;
class AITCharacter;
class UITAbilitySystemComponent;
class UITWeaponManagerComponent;
class UITHealthSet;

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
	UITAbilitySystemComponent* GetITAbilitySystemComponent() const { return AbilitySystemComponent; }

	UFUNCTION(BlueprintCallable, Category = "ITPlayerState|Weapon")
	UITWeaponManagerComponent* GetITWeaponManagerComponent() const { return WeaponManagerComponent; }

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

protected:
	UPROPERTY()
	TObjectPtr<const UITHealthSet> HealthSet;

private:
	UPROPERTY(VisibleAnywhere, Category = "ITPlayerState|AbilitySystemComponent")
	TObjectPtr<UITAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(VisibleAnywhere, Category = "ITPlayerState|Component")
	TObjectPtr<UITWeaponManagerComponent> WeaponManagerComponent;

	UFUNCTION()
	void OnReadyPawnData(APlayerState* Player, APawn* NewPawn, APawn* OldPawn);

	void InitAttributeSet(UDataTable* InitDataTable);

	FITAbilitySet_GrantedHandles GrantedHandles;
};
