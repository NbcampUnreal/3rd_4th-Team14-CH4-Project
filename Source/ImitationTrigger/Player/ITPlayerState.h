#pragma once

#include "GameplayEffectTypes.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystem/ITAbilitySet.h"
#include "GameFramework/PlayerState.h"
#include "AttributeSet.h"
#include "ITPlayerState.generated.h"

class UITItemManagerComponent;
class AITPlayerController;
class AITCharacter;
class UITAbilitySystemComponent;
class UITWeaponManagerComponent;
class UITHealthSet;
class UITAmmoSet;
class UITCombatSet;
class UITPawnData;
class UITPawnDataList;
struct FOnAttributeChangeData;

UCLASS()
class IMITATIONTRIGGER_API AITPlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AITPlayerState(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	virtual void BeginPlay() override;
	virtual void EndPlay(EEndPlayReason::Type EndPlayReason) override;
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(BlueprintCallable, Category = "ITPlayerState")
	AITPlayerController* GetITPlayerController() const;

	UFUNCTION(BlueprintCallable, Category = "ITPlayerState")
	AITCharacter* GetITCharacter() const;

	UFUNCTION(BlueprintCallable, Category = "ITPlayerState")
	UITAbilitySystemComponent* GetITAbilitySystemComponent() const { return AbilitySystemComponent; }

	UFUNCTION(BlueprintCallable, Category = "ITPlayerState")
	UITWeaponManagerComponent* GetITWeaponManagerComponent() const { return WeaponManagerComponent; }

	UFUNCTION(BlueprintCallable, Category = "ITPlayerState")
	UITItemManagerComponent* GetITItemManagerComponent() const { return ItemManagerComponent; }

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	const UITPawnData* GetPawnData() const;

	void SetStartTimeSeconds(float InStartTimeSeconds);
	void SetEndTimeSeconds(float InEndTimeSeconds);
	float GetSurviveTimeSeconds() const;

	void SetRank(int32 InRank) { Rank = InRank; }
	int32 GetRank() const { return Rank; }

	// 생존 여부 확인
	UFUNCTION(BlueprintCallable, Category = "ITPlayerState")
	bool IsAlive() const { return bIsAlive; }

	UPROPERTY(BlueprintReadOnly, Replicated)
	bool bIsAlive;

protected:
	virtual bool ReplicateSubobjects(class UActorChannel* Channel, class FOutBunch* Bunch, FReplicationFlags* RepFlags) override;

	UPROPERTY()
	TObjectPtr<const UITHealthSet> HealthSet;

	UPROPERTY()
	TObjectPtr<const UITAmmoSet> AmmoSet;

	UPROPERTY()
	TObjectPtr<const UITCombatSet> CombatSet;

	UPROPERTY(Replicated)
	int32 Rank;

	UPROPERTY(Replicated)
	float StartTimeSeconds;

	UPROPERTY(Replicated)
	float EndTimeSeconds;

	UPROPERTY(Replicated)
	const UITPawnData* CachedPawnData;

private:
	UPROPERTY(VisibleAnywhere, Category = "ITPlayerState|AbilitySystemComponent")
	TObjectPtr<UITAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(VisibleAnywhere, Category = "ITPlayerState|Component")
	TObjectPtr<UITWeaponManagerComponent> WeaponManagerComponent;

	UPROPERTY(VisibleAnywhere, Category = "ITPlayerState|Component")
	TObjectPtr<UITItemManagerComponent> ItemManagerComponent;

	UFUNCTION()
	void OnReadyPawnData(APlayerState* Player, APawn* NewPawn, APawn* OldPawn);

	void InitAttributeSet(UDataTable* InitDataTable);
	void BindAttributeDelegate();

	void OnHealthChanged(const FOnAttributeChangeData& Data);

	void InitAbilitySystemComponent();
	void BindWeaponChanged();

	FITAbilitySet_GrantedHandles GrantedHandles;
};
