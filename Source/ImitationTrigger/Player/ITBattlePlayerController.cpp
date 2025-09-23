#include "Player/ITBattlePlayerController.h"
#include "AbilitySystem/ITAbilitySystemComponent.h"
#include "AbilitySystem/Attributes/ITHealthSet.h"
#include "UI/HUDWidget.h"


AITBattlePlayerController::AITBattlePlayerController()
{
}

void AITBattlePlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (IsValid(HUDWidgetClass) && HUDWidget == nullptr)
	{
		HUDWidget = CreateWidget<UHUDWidget>(this, HUDWidgetClass);
		if (IsValid(HUDWidget))
		{
			HUDWidget->AddToViewport();
			InitHUD();
		}
	}
}

void AITBattlePlayerController::EndPlay(EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	if (IsValid(HUDWidget) && HUDWidget->IsInViewport())
	{
		HUDWidget->RemoveFromParent();
	}
}

void AITBattlePlayerController::InitHUD()
{
	UITAbilitySystemComponent* ITASC = GetITAbilitySystemComponent();
	if (IsValid(ITASC))
	{
		if (IsValid(HUDWidget))
		{
			BindAttributeChangeDelegate(ITASC, UITHealthSet::GetHealthAttribute(), this, &ThisClass::OnHealthChanged);
			BindAttributeChangeDelegate(ITASC, UITHealthSet::GetMaxHealthAttribute(), this, &ThisClass::OnMaxHealthChanged);
			BindAttributeChangeDelegate(ITASC, UITHealthSet::GetShieldAttribute(), this, &ThisClass::OnShieldChanged);
			BindAttributeChangeDelegate(ITASC, UITHealthSet::GetMaxShieldAttribute(), this, &ThisClass::OnMaxShieldChanged);
		}
	}

	UpdateHealth();
	UpdateShield();
}

void AITBattlePlayerController::OnHealthChanged(const FOnAttributeChangeData& Data)
{
	UpdateHealth();
}

void AITBattlePlayerController::OnMaxHealthChanged(const FOnAttributeChangeData& Data)
{
	UpdateHealth();
}

void AITBattlePlayerController::UpdateHealth()
{
	UITAbilitySystemComponent* ITASC = GetITAbilitySystemComponent();
	if (IsValid(ITASC))
	{
		if (IsValid(HUDWidget))
		{
			const float Health = ITASC->GetNumericAttribute(UITHealthSet::GetHealthAttribute());
			const float MaxHealth = ITASC->GetNumericAttribute(UITHealthSet::GetMaxHealthAttribute());
			HUDWidget->UpdateHealth(Health, MaxHealth);
		}
	}
}

void AITBattlePlayerController::OnShieldChanged(const FOnAttributeChangeData& Data)
{
	UpdateShield();
}

void AITBattlePlayerController::OnMaxShieldChanged(const FOnAttributeChangeData& Data)
{
	UpdateShield();
}

void AITBattlePlayerController::UpdateShield()
{
	UITAbilitySystemComponent* ITASC = GetITAbilitySystemComponent();
	if (IsValid(ITASC))
	{
		if (IsValid(HUDWidget))
		{
			const float Shield = ITASC->GetNumericAttribute(UITHealthSet::GetShieldAttribute());
			const float MaxShield = ITASC->GetNumericAttribute(UITHealthSet::GetMaxShieldAttribute());
			HUDWidget->UpdateShield(Shield, MaxShield);
		}
	}
}
