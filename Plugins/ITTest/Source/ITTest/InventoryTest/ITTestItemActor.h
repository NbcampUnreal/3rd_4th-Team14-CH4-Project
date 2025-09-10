// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ITTestItemActor.generated.h"

class UITTestItemInstance;
class USphereComponent;

UCLASS()
class ITTEST_API AITTestItemActor : public AActor
{
	GENERATED_BODY()

public:
	AITTestItemActor();

	void InitializeItem(UITTestItemInstance* InitItemInstance);

protected:
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnRep_ItemInstance();

	UFUNCTION()
	void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	                     UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep,
	                     const FHitResult& SweepResult);

	UFUNCTION()
	void UpdateAppearance();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "IT|Component")
	TObjectPtr<USphereComponent> SphereComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "IT|Component")
	TObjectPtr<UStaticMeshComponent> StaticMeshComponent;

	UPROPERTY(ReplicatedUsing = OnRep_ItemInstance)
	TObjectPtr<UITTestItemInstance> ItemInstance;
};
