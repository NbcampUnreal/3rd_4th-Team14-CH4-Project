#include "Item/IT_ItemBase_Mine.h"
#include "Components/SphereComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundBase.h"
#include "TimerManager.h"

AIT_ItemBase_Mine::AIT_ItemBase_Mine()
{
	// 상속한 클래스의 Collision 사용
	if (Collision)
	{
		Collision->SetSphereRadius(TriggerRadius);
		Collision->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	}

	PrimaryActorTick.bCanEverTick = false;
}

void AIT_ItemBase_Mine::BeginPlay()
{
	Super::BeginPlay();

	// 도화선(점화)VFX를 아이템에 붙여 재생
	if (FuseVFX)
	{
		UNiagaraFunctionLibrary::SpawnSystemAttached(
			FuseVFX,
			GetRootComponent(),
			NAME_None,
			FVector::ZeroVector,
			FRotator::ZeroRotator,
			EAttachLocation::KeepRelativeOffset,
			true
		);
	}
}

void AIT_ItemBase_Mine::OnItemOverlap(
	UPrimitiveComponent* OverlappedComp,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	// 일단 쿨다운 이펙트 대신 하드코딩으로 한번만 폭발하게 구현.
	if (!bArmed && OtherActor && OtherActor != this)
	{
		bArmed = true;

		// 딜레이 후 폭발
		GetWorld()->GetTimerManager().SetTimer(
			ExplosionTimer,
			this,
			&AIT_ItemBase_Mine::Explode,
			FMath::Max(0.f, ExplosionDelay),
			false
		);
	}
}

void AIT_ItemBase_Mine::Explode()
{
	if (ExplosionVFX)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(
			GetWorld(),
			ExplosionVFX,
			GetActorLocation(),
			FRotator::ZeroRotator
		);
	}

	if (ExplosionSound)
	{
		UGameplayStatics::PlaySoundAtLocation(
			GetWorld(),
			ExplosionSound,
			GetActorLocation()
		);
	}

	UGameplayStatics::ApplyRadialDamage(
		GetWorld(),
		static_cast<float>(ExplosionDamage),
		GetActorLocation(),
		ExplosionRadius,
		UDamageType::StaticClass(),
		TArray<AActor*>(),     
		this,
		GetInstigatorController(),
		true                        
	);

	Destroy();
}
