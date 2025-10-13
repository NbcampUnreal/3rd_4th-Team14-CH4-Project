#pragma once

#include "Player/ITPlayerController.h"
#include "AbilitySystemComponent.h"
#include "AttributeSet.h"
#include "GameplayEffectTypes.h"
#include "Item/Weapon/ITWeaponManagerComponent.h"
#include "ITBattlePlayerController.generated.h"

class UHUDWidget;
class UUserWidget;
class UAbilitySystemComponent;
class UITItemInstance;
class UResultWidget;

UCLASS()
class IMITATIONTRIGGER_API AITBattlePlayerController : public AITPlayerController
{
	GENERATED_BODY()
	
public:
	AITBattlePlayerController();
	virtual void BeginPlay() override;
	virtual void EndPlay(EEndPlayReason::Type EndPlayReason) override;
	virtual void PostNetInit() override;
	virtual void OnRep_PlayerState() override;
	virtual void SetPawn(APawn* InPawn) override;

	UFUNCTION(BlueprintCallable)
	void ToggleMapWidget();

	UFUNCTION(BlueprintCallable)
	void ShowMapWidget();

	UFUNCTION(BlueprintCallable)
	void HideMapWidget();

	UFUNCTION(Server, Reliable)
	void ServerRPC_SetCharacterIndex(int32 CharIndex);

	UFUNCTION(Client, Unreliable)
	void ClientRPC_PlayHitMarkerAnimation();

	UFUNCTION(Client, Unreliable)
	void ClientRPC_PlayKillMarkerAnimation();

	UFUNCTION(Client, Unreliable)
	void ClientRPC_OnFireAnimation();

	UFUNCTION(Client, Reliable)
	void ClientRPC_AddNotify(const FText& KillPlayer, const FText& DiePlayer);

	UFUNCTION(Client, Reliable)
	void ClientRPC_AddKillLog(UTexture2D* KillCharacter, const FText& KillName, UTexture2D* DieCharacter, const FText& DieName, UTexture2D* KillWeapon);

	void OnUpdateAreaInfo(int32 CurrentRoundNumber, int32 AreaTime, float Distance, bool bIsWait);

	UFUNCTION(BlueprintCallable, Client, Reliable)
	void ClientRPC_OnUseActiveSkill(float Cooldown);

	UFUNCTION(Server, Reliable)
	void ServerRPC_RequestAlivePlayerCount();

	UFUNCTION(BlueprintCallable, Client, Reliable)
	void ClientRPC_ChangeAlivePlayerCount(int32 Count);

	// 결과창 표시 (클라이언트 RPC)
	UFUNCTION(Client, Reliable)
	void ClientShowResult(const FString& WinnerName, int32 TotalPlayers);

	UFUNCTION(Server, Reliable)
	void ServerRPC_ReturnToLobby(); //에디터 테스트 용 로비 돌아가기

	UFUNCTION(Server, Reliable)
	void ServerRPC_SetPlayerNickname(const FString& Nickname);

	void SetHUDUsingPawnData(const FText& PlayerName, UTexture2D* PlayerIcon, UTexture2D* ActiveSkillIcon, UTexture2D* UltimateSkillIcon);

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Meta = (AllowPrivateAccess = true))
	TSubclassOf<UHUDWidget> HUDWidgetClass;

	UPROPERTY()
	TObjectPtr<UHUDWidget> HUDWidget;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Meta = (AllowPrivateAccess = true))
	TSubclassOf<UUserWidget> MapWidgetClass;

	UPROPERTY()
	TObjectPtr<UUserWidget> MapWidget;

	// 결과창 위젯 클래스
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
	TSubclassOf<UResultWidget> ResultWidgetClass;

	UPROPERTY()
	TObjectPtr<UResultWidget> ResultWidget;

	UPROPERTY()
	int32 SelectedCharacterIndex = -1;

private:
	void InitWidgets();
	void ReleaseWidgets();

	void CreatePlayerWidgets();
	void InitHUD();

	template<class UClass, typename FuncType>
	void BindAttributeChangeDelegate(UAbilitySystemComponent* ASC, FGameplayAttribute Attribute, UClass Class, FuncType Func);

	void OnHealthChanged(const FOnAttributeChangeData& Data);
	void OnMaxHealthChanged(const FOnAttributeChangeData& Data);
	void UpdateHealth();

	void OnShieldChanged(const FOnAttributeChangeData& Data);
	void OnMaxShieldChanged(const FOnAttributeChangeData& Data);
	void UpdateShield();

	void OnAmmoChanged(const FOnAttributeChangeData& Data);
	void OnNormalAmmoChanged(const FOnAttributeChangeData& Data);
	void OnSpecialAmmoChanged(const FOnAttributeChangeData& Data);
	void UpdateAmmo();

	void OnKillCountChanged(const FOnAttributeChangeData& Data);
	void UpdateKillCount();

	UFUNCTION()
	void OnMainWeaponUpdate(UITItemInstance* ItemInstance);

	UFUNCTION()
	void OnSubWeaponUpdate(UITItemInstance* ItemInstance);

	UFUNCTION()
	void OnCurrentWeaponUpdate(ECurrentWeaponSlot CurrentWeaponType);

	UFUNCTION()
	void OnCurrentHelmetUpdate(int32 CurrentHelmetTier);

	UFUNCTION()
	void OnCurrentArmorUpdate(int32 CurrentArmorTier);


	bool bISFirstWeapon = true;
};

template<class UClass, typename FuncType>
inline void AITBattlePlayerController::BindAttributeChangeDelegate(UAbilitySystemComponent* ASC, FGameplayAttribute Attribute, UClass Class, FuncType Func)
{
	ASC->GetGameplayAttributeValueChangeDelegate(Attribute).AddUObject(Class, Func);
}
