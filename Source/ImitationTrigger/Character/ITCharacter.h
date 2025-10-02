#pragma once

#include "AbilitySystemInterface.h"
#include "Cosmetics/ITCharacterPartType.h"
#include "GameFramework/Character.h"
#include "GameplayTagContainer.h"
#include "ITCharacter.generated.h"

class AITPlayerController;
class AITPlayerState;
class UITPawnData;
class UITPawnDataList;
class UITCameraComponent;
class UITHeroComponent;
class UITCharacterPartComponent;
class UITCharacterAnimComponent;

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
	virtual void PossessedBy(AController* NewController) override;

	const UITPawnData* GetPawnData() const { return PawnData; }
	void SetPawnDataByIndex(int32 Index);
	TArray<FITCharacterPartHandle>& GetCharacterPartHandles() { return CharacterPartHandles; }

	FGameplayTagContainer GetASCGameplayTags() const;

	void AddInitCharacterPartsAtServer();
	void SetBodyMeshes();
	void SetAnimLayerRules();

	UITCharacterPartComponent* GetITCharacterPartComponent();
	UITCharacterAnimComponent* GetITCharacterAnimComponent();

	UPROPERTY(EditDefaultsOnly, Replicated, Category = "PawnData")
	TObjectPtr<const UITPawnData> PawnData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = "PawnData")
	TObjectPtr<UITPawnDataList> PawnDataList;

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "IT|Character")
	TObjectPtr<UITCameraComponent> CameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "IT|Character")
	TObjectPtr<UITHeroComponent> HeroComponent;

private:
	TArray<FITCharacterPartHandle> CharacterPartHandles;
};
