#pragma once

#include "CoreMinimal.h"
#include "Item/IT_ItemBase.h"
#include "IT_ItemBase_Grenade.generated.h"

class UNiagaraSystem;
class USphereComponent;

UCLASS()
class IMITATIONTRIGGER_API AIT_ItemBase_Grenade : public AIT_ItemBase
{
    GENERATED_BODY()

public:
    AIT_ItemBase_Grenade();

protected:
    virtual void BeginPlay() override;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UStaticMeshComponent* MeshComp;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    USphereComponent* ExplosionRadiusComp;

    UPROPERTY(EditDefaultsOnly, Category = "FX")
    UNiagaraSystem* ExplosionVFX;

    UPROPERTY(EditDefaultsOnly, Category = "Grenade")
    float FuseTime = 2.5f;

    UPROPERTY(EditDefaultsOnly, Category = "Grenade")
    float ExplosionDamage = 50.f;

    UPROPERTY(EditDefaultsOnly, Category = "Grenade")
    float ExplosionRadius = 400.f;

    UPROPERTY(EditDefaultsOnly, Category = "Sound")
    USoundBase* ExplosionSound;
    
    UPROPERTY(EditDefaultsOnly, Category = "Sound")
    USoundBase* BounceSound;

    FTimerHandle TimerHandle_Explode;

protected:
    UFUNCTION()
    void OnGrenadeHit(UPrimitiveComponent* HitComp, AActor* OtherActor,
        UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

    // ���� ó�� : �������� ó����.
    void Explode();

    // Ŭ�� ��ü�� ���� FX �ݿ�
    UFUNCTION(NetMulticast, Reliable)
    void Multicast_PlayExplosionFX();
};
