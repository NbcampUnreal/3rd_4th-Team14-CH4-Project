// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AbilitySystem/ITAbilitySet.h"
#include "Cosmetics/ITCharacterPartType.h"
#include "ITWeaponManagerComponent.generated.h"

class AITItemActor;
class UITItemInstance;

UENUM(BlueprintType)
enum class ECurrentWeaponSlot : uint8
{
	None,
	MainWeapon,
	SubWeapon
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCurrentWeaponTypeChanged, ECurrentWeaponSlot, CurrentWeaponType);

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

	UFUNCTION(BlueprintCallable, Category = "IT|Weapon")
	UITItemInstance* GetMainWeaponInstance() const { return MainWeaponInstance; }

	UFUNCTION(BlueprintCallable, Category = "IT|Weapon")
	UITItemInstance* GetSubWeaponInstance() const { return SubWeaponInstance; }

	UFUNCTION(BlueprintCallable, Category = "IT|Weapon")
	int32 GetReserveAmmo() const;

	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "IT|Weapon")
	void ServerRPC_PickupWeapon(UITItemInstance* NewWeaponInstance);

	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "IT|Weapon")
	void ServerRPC_ChangeWeapon(ECurrentWeaponSlot WeaponToChange);

	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "IT|Weapon")
	void ServerRPC_DropCurrentWeapon();

	// 탄약 관련 로직
	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "IT|Weapon")
	void ServerRPC_AddAmmo(int32 Quantity);

	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "IT|Weapon")
	void ServerRPC_ConsumeAmmo(int32 Quantity);

	// UI 델리게이트 프로퍼티
	UPROPERTY(BlueprintAssignable, Category = "IT|Weapon")
	FOnCurrentWeaponTypeChanged OnCurrentWeaponTypeChanged;

	UPROPERTY(BlueprintAssignable, Category = "IT|Weapon")
	FOnMainWeaponChanged OnMainWeaponChanged;

	UPROPERTY(BlueprintAssignable, Category = "IT|Weapon")
	FOnSubWeaponChanged OnSubWeaponChanged;

	void SetCurrentWeaponType(ECurrentWeaponSlot InType);
	void SetMainWeaponInstance(TObjectPtr<UITItemInstance> WeaponInstance);
	void SetSubWeaponInstance(TObjectPtr<UITItemInstance> WeaponInstance);

protected:
	UPROPERTY(ReplicatedUsing = OnRep_CurrentWeaponTypeChanged)
	ECurrentWeaponSlot CurrentWeaponType = ECurrentWeaponSlot::None;

	UPROPERTY(ReplicatedUsing = OnRep_MainWeaponChanged)
	TObjectPtr<UITItemInstance> MainWeaponInstance;

	UPROPERTY(ReplicatedUsing = OnRep_SubWeaponChanged)
	TObjectPtr<UITItemInstance> SubWeaponInstance;

	UPROPERTY()
	ECurrentWeaponSlot PreviousWeaponType = ECurrentWeaponSlot::None;

	UPROPERTY()
	FITAbilitySet_GrantedHandles GrantedHandles;

	UPROPERTY()
	FITCharacterPartHandle WeaponPartHandle;

	// if (인벤토리 구현) { 인벤토리로 이전 + 탄약 종류 추가 }
	UPROPERTY(Replicated)
	int32 ReserveAmmo = 0;

	UFUNCTION()
	void OnRep_CurrentWeaponTypeChanged();

	UFUNCTION()
	void OnRep_MainWeaponChanged();

	UFUNCTION()
	void OnRep_SubWeaponChanged();

private:
	void EquipWeapon();
	void UnequipWeapon();
};
