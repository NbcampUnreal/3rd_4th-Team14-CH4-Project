// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AbilitySystem/ITAbilitySet.h"
#include "Cosmetics/ITCharacterPartType.h"
#include "ITWeaponManagerComponent.generated.h"

class AITCharacter;
class AITPlayerState;
class UITItemInstance;

UENUM(BlueprintType)
enum class ECurrentWeaponSlot : uint8
{
	None,
	MainWeapon,
	SubWeapon
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCurrentWeaponChanged, ECurrentWeaponSlot, CurrentWeapon);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMainWeaponChanged, UITItemInstance*, MainWeaponInstance);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSubWeaponChanged, UITItemInstance*, SubWeaponInstance);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class IMITATIONTRIGGER_API UITWeaponManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UITWeaponManagerComponent();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(BlueprintCallable, Category = "IT|Weapon")
	UITItemInstance* GetCurrentWeapon() const;

	// 빈슬롯에 자동 장착, 가득 찬 경우 현재 무기와 교체
	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "IT|Weapon")
	void ServerRPC_PickupWeapon(UITItemInstance* NewWeaponInstance);

	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "IT|Weapon")
	void ServerRPC_SwapWeapon();

	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "IT|Weapon")
	void ServerRPC_DropCurrentWeapon();

	// UI 델리게이트 프로퍼티
	UPROPERTY(BlueprintAssignable, Category = "IT|Weapon")
	FOnCurrentWeaponChanged OnCurrentWeaponChanged;

	UPROPERTY(BlueprintAssignable, Category = "IT|Weapon")
	FOnMainWeaponChanged OnMainWeaponChanged;

	UPROPERTY(BlueprintAssignable, Category = "IT|Weapon")
	FOnSubWeaponChanged OnSubWeaponChanged;

protected:
	UPROPERTY(ReplicatedUsing = OnRep_CurrentWeaponChanged)
	ECurrentWeaponSlot CurrentWeapon = ECurrentWeaponSlot::None;

	UPROPERTY(ReplicatedUsing = OnRep_CurrentMainWeaponChanged)
	TObjectPtr<UITItemInstance> MainWeaponInstance;

	UPROPERTY(ReplicatedUsing = OnRep_CurrentSubWeaponChanged)
	TObjectPtr<UITItemInstance> SubWeaponInstance;

	UPROPERTY()
	FITAbilitySet_GrantedHandles GrantedHandles;

	UPROPERTY()
	FITCharacterPartHandle WeaponPartHandle;

	UFUNCTION()
	void OnRep_CurrentWeaponChanged();

	UFUNCTION()
	void OnRep_CurrentMainWeaponChanged();

	UFUNCTION()
	void OnRep_CurrentSubWeaponChanged();

private:
	void EquipWeapon();
	void UnequipWeapon();
};
