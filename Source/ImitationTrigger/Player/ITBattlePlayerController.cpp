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
			BindAttributeChangeDelegate(ITASC, UITHealthSet::GetMaxHealthAttribute(), this, &ThisClass::OnHealthChanged);
		}
	}
}

void AITBattlePlayerController::OnHealthChanged(const FOnAttributeChangeData& Data)
{
}

void AITBattlePlayerController::OnMaxHealthChanged(const FOnAttributeChangeData& Data)
{
}
