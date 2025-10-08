#include "AbilitySystem/Attributes/ITHealthSet.h"
#include "AbilitySystem/Attributes/ITCombatSet.h"
#include "Net/UnrealNetwork.h"
#include "Character/ITPawnData.h"
#include "Player/ITPlayerState.h"
#include "Player/ITBattlePlayerController.h"
#include "Item/Weapon/ITWeaponManagerComponent.h"
#include "Item/ITItemInstance.h"
#include "Item/Weapon/ITItemDefinition_Weapon.h"
#include "GameModes/ITBattleGameMode.h"
#include "AbilitySystem/ITAbilitySystemComponent.h"
#include "GameplayEffectExtension.h"

UITHealthSet::UITHealthSet()
{
	InitHealth(0.0f);
	InitMaxHealth(0.0f);
	InitShield(0.0f);
	InitMaxShield(0.0f);
	InitHeadshotResistances(0.0f);
	InitGainHealth(0.0f);
	InitGainShield(0.0f);
	InitGainDamage(0.0f);
	InitGainHeadshotDamage(0.0f);
}

void UITHealthSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, MaxHealth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, Shield, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, MaxShield, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, HeadshotResistances, COND_None, REPNOTIFY_Always);
}

bool UITHealthSet::PreGameplayEffectExecute(FGameplayEffectModCallbackData& Data)
{
	if (!Super::PreGameplayEffectExecute(Data))
	{
		return false;
	}

	BeforeHealth = GetHealth();
	BeforeMaxHealth = GetMaxHealth();
	BeforeShield = GetShield();
	BeforeMaxShield = GetMaxShield();

	return true;
}

void UITHealthSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	const FGameplayEffectContextHandle& EffectContext = Data.EffectSpec.GetEffectContext();
	AActor* Instigator = EffectContext.GetOriginalInstigator();
	AActor* Causer = EffectContext.GetEffectCauser();

	float RealDealtAmount = 0.0f;

	if (Data.EvaluatedData.Attribute == GetGainDamageAttribute())
	{
		const float DamageValue = GetGainDamage();
		RealDealtAmount += ApplyDamageAndReturnRealDealtAmount(DamageValue);
		SetGainDamage(0.0f);
	}
	else if (Data.EvaluatedData.Attribute == GetGainHeadshotDamageAttribute())
	{
		const float HeadDamageValue = GetGainHeadshotDamage();
		const float DamageValue = CalculateHeadshotDamage(HeadDamageValue);
		RealDealtAmount += ApplyDamageAndReturnRealDealtAmount(DamageValue);
		SetGainHeadshotDamage(0.0f);
	}
	else if (Data.EvaluatedData.Attribute == GetGainHealthAttribute())
	{
		const float RestoreValue = GetGainHealth();
		const float NewValue = FMath::Clamp(GetHealth() + RestoreValue, 0, GetMaxHealth());
		SetHealth(NewValue);
		SetGainHealth(0.0f);
	}
	else if (Data.EvaluatedData.Attribute == GetGainShieldAttribute())
	{
		const float RestoreValue = GetGainShield();
		const float NewValue = FMath::Clamp(GetShield() + RestoreValue, 0, GetMaxShield());
		SetShield(NewValue);
		SetGainShield(0.0f);
	}

	if (RealDealtAmount > 0)
	{
		AITPlayerState* AttackerPlayerState = Cast<AITPlayerState>(Instigator);
		if (IsValid(AttackerPlayerState))
		{
			UAbilitySystemComponent* AttackerASC = AttackerPlayerState->GetAbilitySystemComponent();
			AccumulateDamageDealt(AttackerASC, RealDealtAmount);

			if (GetHealth() <= 0)
			{
				AccumulateKillCount(AttackerASC);
				AITBattlePlayerController* AttackerController = Cast<AITBattlePlayerController>(AttackerPlayerState->GetOwningController());

				AActor* TargetActor = Data.Target.GetOwnerActor();
				if (IsValid(TargetActor))
				{
					AITPlayerState* TargetPlayerState = Cast<AITPlayerState>(TargetActor);
					if (IsValid(TargetPlayerState))
					{
						AITBattlePlayerController* TargetController = Cast<AITBattlePlayerController>(TargetPlayerState->GetOwningController());
						NotifyKillToClients(AttackerController, TargetController);
					}
				}
			}
		}
	}

	if (GetHealth() != BeforeHealth)
	{
		OnHealthChanged.Broadcast(Instigator, Causer, &(Data.EffectSpec), Data.EvaluatedData.Magnitude, BeforeHealth, GetHealth());
		
		if (GetHealth() <= 0)
		{
			OnOutOfHealth.Broadcast(Instigator, Causer, &(Data.EffectSpec), Data.EvaluatedData.Magnitude, BeforeHealth, GetHealth());
		}
	}
	if (GetMaxHealth() != BeforeMaxHealth)
	{
		OnMaxHealthChanged.Broadcast(Instigator, Causer, &(Data.EffectSpec), Data.EvaluatedData.Magnitude, BeforeMaxHealth, GetMaxHealth());
	}
	if (GetShield() != BeforeShield)
	{
		OnShieldChanged.Broadcast(Instigator, Causer, &(Data.EffectSpec), Data.EvaluatedData.Magnitude, BeforeShield, GetShield());
		
		if (GetShield() <= 0)
		{
			OnOutOfShield.Broadcast(Instigator, Causer, &(Data.EffectSpec), Data.EvaluatedData.Magnitude, BeforeShield, GetShield());
		}
	}
	if (GetMaxShield() != BeforeMaxShield)
	{
		OnMaxShieldChanged.Broadcast(Instigator, Causer, &(Data.EffectSpec), Data.EvaluatedData.Magnitude, BeforeMaxShield, GetMaxShield());
	}
}

void UITHealthSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	ClampAttribute(Attribute, NewValue);
}

void UITHealthSet::PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const
{
	Super::PreAttributeBaseChange(Attribute, NewValue);

	ClampAttribute(Attribute, NewValue);
}

void UITHealthSet::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
{
	Super::PostAttributeChange(Attribute, OldValue, NewValue);
}

void UITHealthSet::PostAttributeBaseChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) const
{
	Super::PostAttributeBaseChange(Attribute, OldValue, NewValue);
}

void UITHealthSet::ClampAttribute(const FGameplayAttribute& Attribute, float& NewValue) const
{
	if (Attribute == GetHealthAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0, GetMaxHealth());
	}
	else if (Attribute == GetShieldAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0, GetMaxShield());
	}
}

float UITHealthSet::CalculateHeadshotDamage(float InDamage)
{
	const float ReducedRate = FMath::Max(0.0f, 1.0 - GetHeadshotResistances());
	return InDamage * ReducedRate;
}

float UITHealthSet::ApplyDamageAndReturnRealDealtAmount(float InDamage)
{
	float RemainDamage = InDamage;
	float DealtAmount = 0;

	const float ReducedShield = FMath::Min(GetShield(), RemainDamage);
	SetShield(GetShield() - ReducedShield);
	RemainDamage -= ReducedShield;
	DealtAmount += ReducedShield;
	
	if (!FMath::IsNearlyZero(RemainDamage))
	{
		const float ReducedHealth = FMath::Min(GetHealth(), RemainDamage);
		SetHealth(GetHealth() - ReducedHealth);
		DealtAmount += ReducedHealth;
	}
	return DealtAmount;
}

void UITHealthSet::AccumulateDamageDealt(UAbilitySystemComponent* ASC, float DamageDealt)
{
	if (IsValid(ASC))
	{
		float OldValue = ASC->GetNumericAttribute(UITCombatSet::GetDamageDealtAttribute());
		float NewValue = OldValue + DamageDealt;
		ASC->SetNumericAttributeBase(UITCombatSet::GetDamageDealtAttribute(), NewValue);
	}
}

void UITHealthSet::AccumulateKillCount(UAbilitySystemComponent* ASC)
{
	if (IsValid(ASC))
	{
		float OldValue = ASC->GetNumericAttribute(UITCombatSet::GetKillCountAttribute());
		float NewValue = OldValue + 1;
		ASC->SetNumericAttributeBase(UITCombatSet::GetKillCountAttribute(), NewValue);
	}
}

void UITHealthSet::NotifyKillToClients(AITBattlePlayerController* AttackerController, AITBattlePlayerController* TargetController)
{
	if (!IsValid(AttackerController))
	{
		return;
	}
	if (!IsValid(TargetController))
	{
		return;
	}

	AITPlayerState* AttackerPlayerState = AttackerController->GetITPlayerState();;
	AITPlayerState* TargetPlayerState = TargetController->GetITPlayerState();;

	if (!IsValid(AttackerPlayerState))
	{
		return;
	}
	if (!IsValid(TargetPlayerState))
	{
		return;
	}

	FText KillPlayerName = FText::FromString(AttackerController->PlayerState->GetPlayerName());
	FText DiePlayerName = FText::FromString(TargetController->PlayerState->GetPlayerName());

	UTexture2D* KillCharacter = nullptr;
	if (AttackerPlayerState->GetPawnData())
	{
		KillCharacter = AttackerPlayerState->GetPawnData()->Thumbnail;
	}

	UTexture2D* DieCharacter = nullptr;
	if (TargetPlayerState->GetPawnData())
	{
		DieCharacter = TargetPlayerState->GetPawnData()->Thumbnail;
	}

	UTexture2D* KillWeapon = nullptr;
	UITWeaponManagerComponent* WeaponManagerComponent = AttackerPlayerState->GetITWeaponManagerComponent();
	if (IsValid(WeaponManagerComponent))
	{
		UITItemInstance* WeaponInstance = WeaponManagerComponent->GetCurrentWeapon();
		if (IsValid(WeaponInstance))
		{
			UITItemDefinition* WeaponDefinition = WeaponInstance->GetItemDefinition();
			if (IsValid(WeaponDefinition))
			{
				KillWeapon = WeaponDefinition->ItemIcon;
			}
		}
	}

	AttackerController->ClientRPC_AddNotify(KillPlayerName, DiePlayerName);

	if (AttackerController->GetWorld())
	{
		AITBattleGameMode* BattleGameMode = AttackerController->GetWorld()->GetAuthGameMode<AITBattleGameMode>();
		if (IsValid(BattleGameMode))
		{
			const TArray<TObjectPtr<APlayerController>>& MatchPlayers = BattleGameMode->GetMatchPlayers();
			for (APlayerController* MatchPlayer : MatchPlayers)
			{
				AITBattlePlayerController* ITMatchPlayer = Cast<AITBattlePlayerController>(MatchPlayer);
				if (IsValid(ITMatchPlayer))
				{
					ITMatchPlayer->ClientRPC_AddKillLog(KillCharacter, KillPlayerName, DieCharacter, DiePlayerName, KillWeapon);
				}
			}
		}
	}
}

void UITHealthSet::OnRep_Health(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, Health, OldValue);

	float OldFloatValue = OldValue.GetCurrentValue();
	float NewFloatValue = GetHealth();
	float Magnitude = NewFloatValue - OldFloatValue;
	OnHealthChanged.Broadcast(nullptr, nullptr, nullptr, Magnitude, OldFloatValue, NewFloatValue);

	if (GetHealth() <= 0)
	{
		OnOutOfHealth.Broadcast(nullptr, nullptr, nullptr, Magnitude, OldFloatValue, NewFloatValue);
	}
}

void UITHealthSet::OnRep_MaxHealth(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, MaxHealth, OldValue);

	float OldFloatValue = OldValue.GetCurrentValue();
	float NewFloatValue = GetMaxHealth();
	float Magnitude = NewFloatValue - OldFloatValue;
	OnMaxHealthChanged.Broadcast(nullptr, nullptr, nullptr, Magnitude, OldFloatValue, NewFloatValue);
}

void UITHealthSet::OnRep_Shield(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, Shield, OldValue);

	float OldFloatValue = OldValue.GetCurrentValue();
	float NewFloatValue = GetShield();
	float Magnitude = NewFloatValue - OldFloatValue;
	OnShieldChanged.Broadcast(nullptr, nullptr, nullptr, Magnitude, OldFloatValue, NewFloatValue);

	if (GetShield() <= 0)
	{
		OnOutOfShield.Broadcast(nullptr, nullptr, nullptr, Magnitude, OldFloatValue, NewFloatValue);
	}
}

void UITHealthSet::OnRep_MaxShield(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, MaxShield, OldValue);

	float OldFloatValue = OldValue.GetCurrentValue();
	float NewFloatValue = GetMaxShield();
	float Magnitude = NewFloatValue - OldFloatValue;
	OnMaxShieldChanged.Broadcast(nullptr, nullptr, nullptr, Magnitude, OldFloatValue, NewFloatValue);
}
