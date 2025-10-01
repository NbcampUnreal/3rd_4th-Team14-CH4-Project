// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/IT_ItemBase_HpPotion.h"
#include "Kismet/GameplayStatics.h"

void AIT_ItemBase_HpPotion::OnItemOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != this)
	{
		// TODO: GAS Heal Effect 적용 가능
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), nullptr, GetActorLocation()); // Heal VFX
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), nullptr, GetActorLocation());   // Heal Sound
		Destroy();
	}
}