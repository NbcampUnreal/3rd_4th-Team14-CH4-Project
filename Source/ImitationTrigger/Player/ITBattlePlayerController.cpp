#include "Player/ITBattlePlayerController.h"
#include "Player/ITPlayerState.h"
#include "AbilitySystem/ITAbilitySystemComponent.h"
#include "AbilitySystem/Attributes/ITAmmoSet.h"
#include "AbilitySystem/Attributes/ITHealthSet.h"
#include "AbilitySystem/Attributes/ITCombatSet.h"
#include "System/ITLogChannel.h"
#include "Kismet/GameplayStatics.h"
#include "UI/HUDWidget.h"
#include "UI/ITMinimapCapture.h"
#include "Item/ITItemInstance.h"
#include "Item/Weapon/ITItemDefinition_Weapon.h"
#include "Item/Weapon/ITWeaponManagerComponent.h"
#include "Engine/Engine.h"
#include "Engine/GameViewportClient.h"
#include "Network/ITGameInstance.h"
#include "Character/ITCharacter.h"
#include "Character/ITPawnData.h"
#include "Character/ITPawnDataList.h"
#include "GameModes/ITBattleGameMode.h"
#include "UI/Result/ResultWidget.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/PlayerState.h"
#include "Net/UnrealNetwork.h"
#include "Item/ITItemManagerComponent.h"

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

	if (IsLocalController())
	{
		if (UITGameInstance* GI = Cast<UITGameInstance>(GetGameInstance()))
		{
			FString Nickname = GI->GetPlayerNickname();
			ServerRPC_SetPlayerNickname(Nickname);
		}
	}
}

void AITBattlePlayerController::EndPlay(EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	if (IsLocalController())
	{
		ReleaseWidgets();
	}

	SelectedCharacterIndex = -1;
}

void AITBattlePlayerController::PostNetInit()
{
	Super::PostNetInit();
	if (IsLocalController())
	{
		if (UITGameInstance* GI = Cast<UITGameInstance>(GetGameInstance()))
		{
			int32 CharIndex = GI->GetSelectedCharacterIndex();

			ServerRPC_SetCharacterIndex(CharIndex);
		}
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

void AITBattlePlayerController::SetPawn(APawn* InPawn)
{
	if (HasAuthority() && SelectedCharacterIndex != -1)
	{
		if (AITCharacter* ITChar = Cast<AITCharacter>(InPawn))
		{
			ITChar->SetPawnDataByIndex(SelectedCharacterIndex);
		}
	}

	Super::SetPawn(InPawn);
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

void AITBattlePlayerController::ServerRPC_SetCharacterIndex_Implementation(int32 CharIndex)
{
	SelectedCharacterIndex = CharIndex;

	if (HasAuthority())
	{
		if (AITBattleGameMode* GM = GetWorld()->GetAuthGameMode<AITBattleGameMode>())
		{
			AITCharacter* ITChar = Cast<AITCharacter>(GetPawn());
			ITChar->Destroy();
			SetPawn(nullptr);
			GM->RestartPlayer(this);
		}
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
				BindAttributeChangeDelegate(ITASC, UITAmmoSet::GetAmmoAttribute(), this, &ThisClass::OnAmmoChanged);
				BindAttributeChangeDelegate(ITASC, UITAmmoSet::GetNormalAmmoAttribute(), this, &ThisClass::OnNormalAmmoChanged);
				BindAttributeChangeDelegate(ITASC, UITAmmoSet::GetSpecialAmmoAttribute(), this, &ThisClass::OnSpecialAmmoChanged);
			}
		}
		UpdateHealth();
		UpdateShield();
		UpdateAmmo();

		AITPlayerState* ITPlayerState = GetITPlayerState();
		if (IsValid(ITPlayerState))
		{
			UITWeaponManagerComponent* WeaponComponent = ITPlayerState->GetITWeaponManagerComponent();
			if (IsValid(WeaponComponent))
			{
				WeaponComponent->OnMainWeaponChanged.AddDynamic(this, &ThisClass::OnMainWeaponUpdate);
				WeaponComponent->OnSubWeaponChanged.AddDynamic(this, &ThisClass::OnSubWeaponUpdate);
				WeaponComponent->OnCurrentWeaponTypeChanged.AddDynamic(this, &ThisClass::OnCurrentWeaponUpdate);
			}

			UITItemManagerComponent* ItemManagerComponent = ITPlayerState->GetITItemManagerComponent();
			if (IsValid(ItemManagerComponent))
			{
				ItemManagerComponent->OnCurrentHelmetChanged.AddDynamic(this, &ThisClass::OnCurrentHelmetUpdate);
				ItemManagerComponent->OnCurrentArmorChanged.AddDynamic(this, &ThisClass::OnCurrentArmorUpdate);
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

void AITBattlePlayerController::ClientRPC_OnUseActiveSkill_Implementation(float Cooldown)
{
	if (IsLocalController())
	{
		if (IsValid(HUDWidget))
		{
			HUDWidget->OnSkill(Cooldown);
		}
	}
}

void AITBattlePlayerController::OnAmmoChanged(const FOnAttributeChangeData& Data)
{
	UpdateAmmo();
}

void AITBattlePlayerController::OnNormalAmmoChanged(const FOnAttributeChangeData& Data)
{
	UpdateAmmo();
}

void AITBattlePlayerController::OnSpecialAmmoChanged(const FOnAttributeChangeData& Data)
{
	UpdateAmmo();
}

void AITBattlePlayerController::UpdateAmmo()
{
	UITAbilitySystemComponent* ITASC = GetITAbilitySystemComponent();
	if (IsValid(ITASC))
	{
		if (IsValid(HUDWidget))
		{
			AITPlayerState* ITPlayerState = GetITPlayerState();
			if (!ITPlayerState)
			{
				return;
			}

			UITWeaponManagerComponent* WeaponManagerComponent = ITPlayerState->GetITWeaponManagerComponent();
			if (!WeaponManagerComponent || !WeaponManagerComponent->GetCurrentWeapon())
			{
				HUDWidget->HasNoWeapon();
				return;
			}

			UITItemInstance* CurrentWeapon = WeaponManagerComponent->GetCurrentWeapon();
			
			const float CurrentAmmo = ITASC->GetNumericAttribute(UITAmmoSet::GetAmmoAttribute());
			const float NormalAmmo = ITASC->GetNumericAttribute(UITAmmoSet::GetNormalAmmoAttribute());
			const float SpecialAmmo = ITASC->GetNumericAttribute(UITAmmoSet::GetSpecialAmmoAttribute());

			if (CurrentWeapon->ItemDefinition->AmmoType == EAmmoType::NormalAmmo)
			{
				HUDWidget->UpdateRifleAmmo(CurrentAmmo, NormalAmmo);
			}
			else if (CurrentWeapon->ItemDefinition->AmmoType == EAmmoType::SpecialAmmo)
			{
				HUDWidget->UpdateSniperAmmo(CurrentAmmo, SpecialAmmo);
			}
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

void AITBattlePlayerController::ServerRPC_SetPlayerNickname_Implementation(const FString& Nickname)
{
	if (!HasAuthority())
	{
		return;
	}

	if (APlayerState* PS = GetPlayerState<APlayerState>())
	{
		PS->SetPlayerName(Nickname);
		UE_LOG(LogTemp, Log, TEXT("Player name set to: %s"), *Nickname);
	}
}

void AITBattlePlayerController::ServerRPC_ReturnToLobby_Implementation()
{
	// 서버에서만 실행됨
	if (!HasAuthority())
	{
		return;
	}

#if WITH_EDITOR
	UE_LOG(LogTemp, Warning, TEXT("Server: Returning all players to lobby"));

	if (UWorld* World = GetWorld())
	{
		const FString LobbyMapName = TEXT("IT_TestEntry");
		World->ServerTravel(LobbyMapName); // 모든 클라이언트가 함께 이동
	}
#else
	UE_LOG(LogTemp, Warning, TEXT("Server: PackagedBuild - client should use ClientTravel"));
	// 패키징 환경에서는 클라이언트가 각자 ClientTravel 사용
#endif
}

void AITBattlePlayerController::ClientShowResult_Implementation(const FString& WinnerName, int32 TotalPlayers)
{
	if (!IsLocalController())
	{
		return;
	}

	// 결과창 생성 및 표시
	if (ResultWidgetClass && !ResultWidget)
	{
		ResultWidget = CreateWidget<UResultWidget>(this, ResultWidgetClass);
	}

	if (ResultWidget)
	{
		AITPlayerState* ITPlayerState = GetITPlayerState();
		if (IsValid(ITPlayerState))
		{
			int32 MyRank = ITPlayerState->GetRank();
			UTexture2D* CharacterImage = nullptr;
			FText PlayerName = FText::FromString(ITPlayerState->GetPlayerName());
			int32 SurvivalTime = (int32)(ITPlayerState->GetSurviveTimeSeconds());
			int32 KillCount = 0;
			float Damage = 0.0f;

			UITAbilitySystemComponent* ASC = ITPlayerState->GetITAbilitySystemComponent();
			if (IsValid(ASC))
			{
				KillCount = (int32)ASC->GetNumericAttribute(UITCombatSet::GetKillCountAttribute());
				Damage = ASC->GetNumericAttribute(UITCombatSet::GetDamageDealtAttribute());
			}

			const UITPawnData* PawnData = ITPlayerState->GetPawnData();
			if (IsValid(PawnData))
			{
				CharacterImage = PawnData->Thumbnail;
			}

			ResultWidget->SetResult(TotalPlayers, MyRank, CharacterImage, PlayerName, SurvivalTime, KillCount, Damage);
		}

		

		ResultWidget->SetWinnerName(WinnerName);
		ResultWidget->AddToViewport(100); // 최상위 레이어에 표시

		FlushPressedKeys();

		// 입력 모드를 UI로 변경
		FInputModeUIOnly InputMode;
		InputMode.SetWidgetToFocus(ResultWidget->TakeWidget());
		SetInputMode(InputMode);
		bShowMouseCursor = true;
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
	else
	{
		// nullptr이면, 이름 및 아이콘 제거
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
	else
	{
		// nullptr이면, 이름 및 아이콘 제거
	}
}

void AITBattlePlayerController::OnCurrentWeaponUpdate(ECurrentWeaponSlot CurrentWeaponType)
{
	/* 0 None 맨손 = 하이라이트 제거
	* 1 MainWeapon = 메인 웨폰 하이라이트 추가
	* 2 SubWeapon = 서브 웨폰 하이라이트 추가
	*/
	UpdateAmmo();
}

void AITBattlePlayerController::OnCurrentHelmetUpdate(int32 CurrentHelmetTier)
{
	if (IsValid(HUDWidget))
	{
		HUDWidget->SetEquipmentIconHelmet(CurrentHelmetTier);
	}
}

void AITBattlePlayerController::OnCurrentArmorUpdate(int32 CurrentArmorTier)
{
	if (IsValid(HUDWidget))
	{
		HUDWidget->SetEquipmentIconArmor(CurrentArmorTier);
	}
}
