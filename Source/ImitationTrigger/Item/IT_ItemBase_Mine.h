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
	
	// VFX �浹 �� ��ȭ �ð��� ������ �Ÿ��� ȿ���� ������.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "VFX")
	UNiagaraSystem* FuseVFX = nullptr;

	// ���� VFX
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "VFX")
	UNiagaraSystem* ExplosionVFX = nullptr;

	// ���� ����
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Sound")
	USoundBase* ExplosionSound = nullptr;

	// �浹 �ݰ�
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Mine")
	float TriggerRadius = 120.f;

	// ���߱��� ������
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Mine")
	float ExplosionDelay = 1.f;

	// ���� ������/�ݰ�
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Mine")
	int32 ExplosionDamage = 50;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Mine")
	float ExplosionRadius = 300.f;

	// �ߺ� Ÿ�̸� ����
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
