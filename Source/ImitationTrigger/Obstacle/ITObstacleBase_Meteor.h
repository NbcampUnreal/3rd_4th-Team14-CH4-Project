#pragma once

#include "CoreMinimal.h"
#include "Obstacle/ITObstacleBase.h"
#include "ITObstacleBase_Meteor.generated.h"

class USphereComponent;
class UNiagaraComponent;
class UNiagaraSystem;

UCLASS()
class IMITATIONTRIGGER_API AITObstacleBase_Meteor : public AITObstacleBase
{
    GENERATED_BODY()

public:
    AITObstacleBase_Meteor();

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    USphereComponent* Collision;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UNiagaraComponent* MeteorVFX;

    // �ϴÿ��� �������� Meteor VFX
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "VFX")
    UNiagaraSystem* FallingVFX;

    // ���� �ε����� �� ���� �� ���׸� VFX
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "VFX")
    UNiagaraSystem* ImpactVFX;

    // ���׿� ���� �� �浹���� ���� �ð�
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Meteor")
    float ImpactDelay = 5.0f;

    // ���ط�
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Meteor")
    float Damage = 50.f;

    FTimerHandle ImpactTimer;


protected:
    virtual void BeginPlay() override;

    UFUNCTION()
    void OnImpact();

    UFUNCTION(NetMulticast, Reliable)
    void Multicast_PlayImpactVFX();
};
