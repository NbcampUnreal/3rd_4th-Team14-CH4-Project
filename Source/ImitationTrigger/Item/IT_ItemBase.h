// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "IT_ItemBase.generated.h"

class USphereComponent;
class UStaticMeshComponent;

UCLASS()
class IMITATIONTRIGGER_API AIT_ItemBase : public AActor
{
	GENERATED_BODY()
	

public:
	AIT_ItemBase();

protected:
	UPROPERTY(VisibleAnywhere, Category = "Components")
	USceneComponent* SceneRoot;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	USphereComponent* Collision;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UStaticMeshComponent* Mesh;

	virtual void BeginPlay() override;


	UFUNCTION()
	virtual void OnItemOverlap(
		UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);

public:
	virtual void Tick(float DeltaTime) override;
};
