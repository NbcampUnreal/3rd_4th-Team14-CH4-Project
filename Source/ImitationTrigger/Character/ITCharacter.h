#pragma once

#include "AbilitySystemInterface.h"
#include "GameFramework/Character.h"
#include "ITCharacter.generated.h"

class AITPlayerController;
class AITPlayerState;
class UITPawnData;
class UITHeroComponent;

UCLASS()
class IMITATIONTRIGGER_API AITCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AITCharacter();

	UFUNCTION(BlueprintCallable, Category = "AITCharacter")
	AITPlayerController* GetITPlayerController() const;

	UFUNCTION(BlueprintCallable, Category = "AITCharacter")
	AITPlayerState* GetITPlayerState() const;

	UFUNCTION(BlueprintCallable, Category = "AITCharacter")
	virtual UITAbilitySystemComponent* GetITAbilitySystemComponent() const;

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	const UITPawnData* GetPawnData() const { return PawnData; }

protected:
	UPROPERTY(EditDefaultsOnly, ReplicatedUsing = OnRep_PawnData, Category = "PawnData")
	TObjectPtr<const UITPawnData> PawnData;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Lyra|Character")
	TObjectPtr<UITHeroComponent> HeroComponent;

private:
	UFUNCTION()
	void OnRep_PawnData();
};
