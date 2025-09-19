// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AbilitySystem/ITAbilitySet.h"
#include "ITWeaponManagerComponent.generated.h"

class UITItemInstance;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class IMITATIONTRIGGER_API UITWeaponManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UITWeaponManagerComponent();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "IT|Weapon")
	void ServerRPC_EquipWeapon(UITItemInstance* WeaponInstance);

	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "IT|Weapon")
	void ServerRPC_UnequipWeapon();

protected:
	UPROPERTY(Replicated)
	TObjectPtr<UITItemInstance> PrimaryWeaponInstance;

	UPROPERTY(Replicated)
	TObjectPtr<UITItemInstance> SecondaryWeaponInstance;

	UPROPERTY()
	FITAbilitySet_GrantedHandles GrantedHandles;
};
