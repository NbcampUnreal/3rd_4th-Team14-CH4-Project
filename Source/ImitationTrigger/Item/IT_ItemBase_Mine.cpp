// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/IT_ItemBase_Mine.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SphereComponent.h"

AIT_ItemBase_Mine::AIT_ItemBase_Mine()
{
	ExplosionDelay = 3.f;
	ExplosionRadius = 300.f;
	ExplosionDamage = 50;
}

void AIT_ItemBase_Mine::OnItemOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != this)
	{
		GetWorld()->GetTimerManager().SetTimer(ExplosionTimer, this, &AIT_ItemBase_Mine::Explode, ExplosionDelay, false);
	}
}

void AIT_ItemBase_Mine::Explode()
{
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), nullptr, GetActorLocation()); // Explosion VFX
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), nullptr, GetActorLocation());   // Explosion Sound

	// 데미지 처리
	UGameplayStatics::ApplyRadialDamage(
		GetWorld(),
		ExplosionDamage,
		GetActorLocation(),
		ExplosionRadius,
		UDamageType::StaticClass(),
		TArray<AActor*>(),
		this,
		nullptr,
		true
	);

	Destroy();
}