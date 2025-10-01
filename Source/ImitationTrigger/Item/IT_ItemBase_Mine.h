// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/IT_ItemBase.h"
#include "IT_ItemBase_Mine.generated.h"

/**
 * 
 */
UCLASS()
class IMITATIONTRIGGER_API AIT_ItemBase_Mine : public AIT_ItemBase
{
	GENERATED_BODY()
	
public:
	AIT_ItemBase_Mine();

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Mine")
	float ExplosionDelay;

	UPROPERTY(EditDefaultsOnly, Category = "Mine")
	float ExplosionRadius;

	UPROPERTY(EditDefaultsOnly, Category = "Mine")
	int32 ExplosionDamage;

	FTimerHandle ExplosionTimer;

	virtual void OnItemOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
		const FHitResult& SweepResult) override;

	void Explode();
};
