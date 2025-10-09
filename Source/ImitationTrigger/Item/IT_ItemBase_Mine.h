#pragma once

#include "CoreMinimal.h"
#include "Item/IT_ItemBase.h"
#include "IT_ItemBase_Mine.generated.h"

class UNiagaraSystem;
class USoundBase;

UCLASS()
class IMITATIONTRIGGER_API AIT_ItemBase_Mine : public AIT_ItemBase
{
	GENERATED_BODY()

public:
	AIT_ItemBase_Mine();

protected:
	
	// VFX 충돌 후 점화 시간에 파지직 거리는 효과를 넣음ㅁ.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "VFX")
	UNiagaraSystem* FuseVFX = nullptr;

	// 폭발 VFX
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "VFX")
	UNiagaraSystem* ExplosionVFX = nullptr;

	// 폭발 사운드
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Sound")
	USoundBase* ExplosionSound = nullptr;

	// 충돌 반경
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Mine")
	float TriggerRadius = 120.f;

	// 폭발까지 딜레이
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Mine")
	float ExplosionDelay = 1.f;

	// 폭발 데미지/반경
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Mine")
	int32 ExplosionDamage = 50;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Mine")
	float ExplosionRadius = 300.f;

	// 중복 타이머 방지
	bool bArmed = false;

	FTimerHandle ExplosionTimer;

protected:
	virtual void BeginPlay() override;

	virtual void OnItemOverlap(
		UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	) override;

	void Explode();
};
