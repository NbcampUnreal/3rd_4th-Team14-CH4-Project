#pragma once

#include "Player/ITPlayerController.h"
#include "AbilitySystemComponent.h"
#include "AttributeSet.h"
#include "GameplayEffectTypes.h"
#include "ITBattlePlayerController.generated.h"

class UHUDWidget;
class UUserWidget;
class UAbilitySystemComponent;
class UITItemInstance;


UCLASS()
class IMITATIONTRIGGER_API AITBattlePlayerController : public AITPlayerController
{
	GENERATED_BODY()
	
public:
	AITBattlePlayerController();
	virtual void BeginPlay() override;
	virtual void EndPlay(EEndPlayReason::Type EndPlayReason) override;

	virtual void OnRep_PlayerState() override;


	UFUNCTION(BlueprintCallable)
	void ToggleMapWidget();

	UFUNCTION(BlueprintCallable)
	void ShowMapWidget();

	UFUNCTION(BlueprintCallable)
	void HideMapWidget();


protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Meta = (AllowPrivateAccess = true))
	TSubclassOf<UHUDWidget> HUDWidgetClass;

	UPROPERTY()
	TObjectPtr<UHUDWidget> HUDWidget;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Meta = (AllowPrivateAccess = true))
	TSubclassOf<UUserWidget> MapWidgetClass;

	UPROPERTY()
	TObjectPtr<UUserWidget> MapWidget;


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

	UFUNCTION()
	void OnMainWeaponUpdate(UITItemInstance* ItemInstance);

	UFUNCTION()
	void OnSubWeaponUpdate(UITItemInstance* ItemInstance);
};

template<class UClass, typename FuncType>
inline void AITBattlePlayerController::BindAttributeChangeDelegate(UAbilitySystemComponent* ASC, FGameplayAttribute Attribute, UClass Class, FuncType Func)
{
	ASC->GetGameplayAttributeValueChangeDelegate(Attribute).AddUObject(Class, Func);
}
