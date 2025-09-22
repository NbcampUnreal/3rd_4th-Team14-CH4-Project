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
	MainWeapon,
	SubWeapon,
	None
};

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

protected:
	UPROPERTY(ReplicatedUsing = OnRep_WeaponUpdate)
	ECurrentWeaponSlot CurrentWeapon = ECurrentWeaponSlot::None;

	UPROPERTY(Replicated)
	TObjectPtr<UITItemInstance> MainWeaponInstance;

	UPROPERTY(Replicated)
	TObjectPtr<UITItemInstance> SubWeaponInstance;

	UPROPERTY()
	FITAbilitySet_GrantedHandles GrantedHandles;

	UPROPERTY()
	FITCharacterPartHandle WeaponPartHandle;

	UFUNCTION()
	void OnRep_WeaponUpdate();

private:
	void EquipWeapon();
	void UnequipWeapon();
};
