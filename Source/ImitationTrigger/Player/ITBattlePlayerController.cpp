#include "Player/ITBattlePlayerController.h"
#include "Player/ITPlayerState.h"
#include "AbilitySystem/ITAbilitySystemComponent.h"
#include "AbilitySystem/Attributes/ITHealthSet.h"
#include "System/ITLogChannel.h"
#include "Kismet/GameplayStatics.h"
#include "UI/HUDWidget.h"
#include "UI/ITMinimapCapture.h"
#include "Item/ITItemInstance.h"
#include "Item/Weapon/ITItemDefinition_Weapon.h"
#include "Item/Weapon/ITWeaponManagerComponent.h"
#include "Engine/Engine.h"
#include "Engine/GameViewportClient.h"


AITBattlePlayerController::AITBattlePlayerController()
{
}

void AITBattlePlayerController::BeginPlay()
{
	Super::BeginPlay();

	// StandAlone 상태라면 OnRep_PlayerState가 호출되지 않으므로, 직접 호출해야 한다.
	if (GetNetMode() == NM_Standalone)
	{
		InitWidgets();
	}
}

void AITBattlePlayerController::EndPlay(EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	if (IsLocalController())
	{
		ReleaseWidgets();
	}
}

void AITBattlePlayerController::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	// 미니맵 Caputer 한번 더 (멀티플레이어 환경에서, 한 번에 Capture가 안되는 경우가 있음)
	AActor* Actor = UGameplayStatics::GetActorOfClass(this, AITMinimapCapture::StaticClass());
	AITMinimapCapture* Capture = Cast<AITMinimapCapture>(Actor);
	if (IsValid(Capture))
	{
		Capture->CaptureOnce();
	}

	// 클라이언트에 PlayerState가 준비 되어야 HUD를 초기화할 수 있다.
	InitWidgets();
	FInputModeGameOnly Mode; 
	SetInputMode(Mode); 
	bShowMouseCursor = false;
	
}

void AITBattlePlayerController::ToggleMapWidget()
{
	if (IsValid(MapWidget))
	{
		if (MapWidget->IsInViewport())
		{
			MapWidget->RemoveFromParent();
		}
		else
		{
			MapWidget->AddToViewport();
		}
	}
}

void AITBattlePlayerController::ShowMapWidget()
{
	if (IsValid(MapWidget) && !MapWidget->IsInViewport())
	{
		MapWidget->AddToViewport();
	}
}

void AITBattlePlayerController::HideMapWidget()
{
	if (IsValid(MapWidget) && MapWidget->IsInViewport())
	{
		MapWidget->RemoveFromParent();
	}
}

void AITBattlePlayerController::ClientRPC_AddNotify_Implementation(const FText& KillPlayer, const FText& DiePlayer)
{
	if (IsValid(HUDWidget))
	{
		HUDWidget->AddNotifyText(KillPlayer, DiePlayer);
	}
}

void AITBattlePlayerController::ClientRPC_AddKillLog_Implementation(UTexture2D* KillCharacter, const FText& KillName, UTexture2D* DieCharacter, const FText& DieName, UTexture2D* KillWeapon)
{
	if (IsValid(HUDWidget))
	{
		HUDWidget->AddKillLog(KillCharacter, KillName, DieCharacter, DieName, KillWeapon);
	}
}

void AITBattlePlayerController::ClientRPC_OnFireAnimation_Implementation()
{
	if (IsValid(HUDWidget))
	{
		HUDWidget->OnFire();
	}
}

void AITBattlePlayerController::ClientRPC_PlayHitMarkerAnimation_Implementation()
{
	if (IsValid(HUDWidget))
	{
		HUDWidget->PlayHitMarkerAnimation();
	}
}

void AITBattlePlayerController::ClientRPC_PlayKillMarkerAnimation_Implementation()
{
	if (IsValid(HUDWidget))
	{
		HUDWidget->PlayKillMarkerAnimation();
	}
}

void AITBattlePlayerController::InitWidgets()
{
	CreatePlayerWidgets();
	if (IsValid(HUDWidget))
	{
		HUDWidget->AddToViewport();
		InitHUD();
	}
}

void AITBattlePlayerController::ReleaseWidgets()
{
	if (IsValid(HUDWidget) && HUDWidget->IsInViewport())
	{
		HUDWidget->RemoveFromParent();
	}
}

void AITBattlePlayerController::CreatePlayerWidgets()
{
	if (IsLocalController())
	{
		if (IsValid(HUDWidgetClass) && HUDWidget == nullptr)
		{
			HUDWidget = CreateWidget<UHUDWidget>(this, HUDWidgetClass);
		}

		if (IsValid(MapWidgetClass) && MapWidget == nullptr)
		{
			MapWidget = CreateWidget<UUserWidget>(this, MapWidgetClass);
		}
	}
}

void AITBattlePlayerController::InitHUD()
{
	if (IsLocalController())
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

		AITPlayerState* ITPlayerState = GetITPlayerState();
		if (IsValid(ITPlayerState))
		{
			UITWeaponManagerComponent* WeaponComponent = ITPlayerState->GetITWeaponManagerComponent();
			if (IsValid(WeaponComponent))
			{
				WeaponComponent->OnMainWeaponChanged.AddDynamic(this, &ThisClass::OnMainWeaponUpdate);
				WeaponComponent->OnSubWeaponChanged.AddDynamic(this, &ThisClass::OnSubWeaponUpdate);
			}
		}
	}
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

void AITBattlePlayerController::OnUpdateAreaInfo(int32 CurrentRoundNumber, int32 AreaTime, float Distance, bool bIsWait)
{
	if (IsValid(HUDWidget))
	{
		HUDWidget->OnUpdateAreaInfo(CurrentRoundNumber, AreaTime, Distance, bIsWait);
	}
}

void AITBattlePlayerController::OnMainWeaponUpdate(UITItemInstance* ItemInstance)
{
	if (IsValid(ItemInstance))
	{
		UITItemDefinition_Weapon* WeaponDefinition = ItemInstance->GetItemDefinition<UITItemDefinition_Weapon>();
		if (IsValid(WeaponDefinition))
		{
			if (IsValid(HUDWidget))
			{
				HUDWidget->UpdateWeaponSlotOne(WeaponDefinition->ItemIcon);
				HUDWidget->SetWeaponOneInfo(WeaponDefinition->ItemName);
			}
		}
	}
}

void AITBattlePlayerController::OnSubWeaponUpdate(UITItemInstance* ItemInstance)
{
	if (IsValid(ItemInstance))
	{
		UITItemDefinition_Weapon* WeaponDefinition = ItemInstance->GetItemDefinition<UITItemDefinition_Weapon>();
		if (IsValid(WeaponDefinition))
		{
			if (IsValid(HUDWidget))
			{
				HUDWidget->UpdateWeaponSlotTwo(WeaponDefinition->ItemIcon);
				HUDWidget->SetWeaponTwoInfo(WeaponDefinition->ItemName);
			}
		}
	}
}

