

#pragma once

#include "CoreMinimal.h"
#include "Item/ITGameplayAbility_ItemBase.h"
#include "ITGameplayAbility_SpawnGrenade.generated.h"

class AIT_ItemBase_Grenade;

UCLASS()
class IMITATIONTRIGGER_API UITGameplayAbility_SpawnGrenade : public UITGameplayAbility_ItemBase
{
	GENERATED_BODY()
	
public:
    UITGameplayAbility_SpawnGrenade();


protected:
    UPROPERTY(EditDefaultsOnly, Category = "Grenade")
    TSubclassOf<AIT_ItemBase_Grenade> GrenadeClass;

    // 던지는 힘 (거리에 영향)
    UPROPERTY(EditDefaultsOnly, Category = "Grenade|Throw")
    float ThrowStrength = 1500.f;   

    // 던질 때 약간 위로 향하도록 각도 보정
    UPROPERTY(EditDefaultsOnly, Category = "Grenade|Throw")
    float ThrowPitchAngle = 15.f;   // 15도 위로 던지기

    // 캐릭터 기준 생성 위치 : 앞이랑 위쪽에서 생성함.
    UPROPERTY(EditDefaultsOnly, Category = "Grenade|Spawn")
    FVector SpawnOffset = FVector(70.f, 0.f, 50.f);

    virtual void ActivateAbility(
        const FGameplayAbilitySpecHandle Handle,
        const FGameplayAbilityActorInfo* ActorInfo,
        const FGameplayAbilityActivationInfo ActivationInfo,
        const FGameplayEventData* TriggerEventData) override;
};
