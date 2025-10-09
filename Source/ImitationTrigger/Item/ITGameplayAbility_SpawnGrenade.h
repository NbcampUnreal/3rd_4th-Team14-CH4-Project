

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

    // ������ �� (�Ÿ��� ����)
    UPROPERTY(EditDefaultsOnly, Category = "Grenade|Throw")
    float ThrowStrength = 1500.f;   

    // ���� �� �ణ ���� ���ϵ��� ���� ����
    UPROPERTY(EditDefaultsOnly, Category = "Grenade|Throw")
    float ThrowPitchAngle = 15.f;   // 15�� ���� ������

    // ĳ���� ���� ���� ��ġ : ���̶� ���ʿ��� ������.
    UPROPERTY(EditDefaultsOnly, Category = "Grenade|Spawn")
    FVector SpawnOffset = FVector(70.f, 0.f, 50.f);

    virtual void ActivateAbility(
        const FGameplayAbilitySpecHandle Handle,
        const FGameplayAbilityActorInfo* ActorInfo,
        const FGameplayAbilityActivationInfo ActivationInfo,
        const FGameplayEventData* TriggerEventData) override;
};
