// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/IT_ItemBase.h"
#include "IT_ItemBase_HpPotion.generated.h"


UCLASS()
class IMITATIONTRIGGER_API AIT_ItemBase_HpPotion : public AIT_ItemBase
{
	GENERATED_BODY()

protected:
	virtual void OnItemOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
		const FHitResult& SweepResult) override;
	
};
