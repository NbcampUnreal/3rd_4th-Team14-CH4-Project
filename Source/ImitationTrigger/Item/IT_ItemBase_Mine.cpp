#include "Item/IT_ItemBase_Mine.h"
#include "Components/SphereComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundBase.h"
#include "TimerManager.h"

AIT_ItemBase_Mine::AIT_ItemBase_Mine()
{
	// ����� Ŭ������ Collision ���
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

	// ��ȭ��(��ȭ)VFX�� �����ۿ� �ٿ� ���
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
	// �ϴ� ��ٿ� ����Ʈ ��� �ϵ��ڵ����� �ѹ��� �����ϰ� ����.
	if (!bArmed && OtherActor && OtherActor != this)
	{
		bArmed = true;

		// ������ �� ����
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
