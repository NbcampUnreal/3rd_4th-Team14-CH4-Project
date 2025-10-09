// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/IT_ItemBase.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"

AIT_ItemBase::AIT_ItemBase()
{
	PrimaryActorTick.bCanEverTick = false;

	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	SetRootComponent(SceneRoot);

	Collision = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
	Collision->InitSphereRadius(50.f);
	Collision->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	Collision->SetupAttachment(SceneRoot);
	Collision->OnComponentBeginOverlap.AddDynamic(this, &AIT_ItemBase::OnItemOverlap);


	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Collision);

	bReplicates = true;
	SetReplicateMovement(true);

}

void AIT_ItemBase::BeginPlay()
{
	Super::BeginPlay();
}

void AIT_ItemBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// 서버 전용으로 오버랩 처리
void AIT_ItemBase::OnItemOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!HasAuthority())
	{
		return;
	}


	if (!OtherActor || OtherActor == this)
	{
		return;
	}
}

