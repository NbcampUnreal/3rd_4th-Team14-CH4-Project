// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "ITGameplayAbility.generated.h"

class AITCharacter;
class UITAbilitySystemComponent;

UCLASS()
class IMITATIONTRIGGER_API UITGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UITGameplayAbility();

protected:
	virtual void ActivateAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData* TriggerEventData
	) override;
	
	virtual void EndAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		bool bReplicateEndAbility,
		bool bWasCancelled
	) override;


protected:
	// Owner Character �������� 
	AITCharacter* GetITCharacter(const FGameplayAbilityActorInfo* ActorInfo) const;

	// AbilitySystemComponent ��������
	UITAbilitySystemComponent* GetITASC(const FGameplayAbilityActorInfo* ActorInfo) const;

	// ���� ������ �ִ��� Ȯ��
	bool HasAuthority(const FGameplayAbilityActivationInfo& ActivationInfo) const;

	// ����� �α�
	//void LogAbilityActivated() const;
	//void LogAbilityEnded(bool bWasCancelled) const;



};

