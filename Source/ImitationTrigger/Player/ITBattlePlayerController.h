#pragma once

#include "Player/ITPlayerController.h"
#include "AbilitySystemComponent.h"
#include "AttributeSet.h"
#include "GameplayEffectTypes.h"
#include "ITBattlePlayerController.generated.h"

class UHUDWidget;
class UAbilitySystemComponent;


UCLASS()
class IMITATIONTRIGGER_API AITBattlePlayerController : public AITPlayerController
{
	GENERATED_BODY()
	
public:
	AITBattlePlayerController();
	virtual void BeginPlay() override;
	virtual void EndPlay(EEndPlayReason::Type EndPlayReason) override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Meta = (AllowPrivateAccess = true))
	TSubclassOf<UHUDWidget> HUDWidgetClass;

	UPROPERTY()
	TObjectPtr<UHUDWidget> HUDWidget;


private:
	void InitHUD();

	template<class UClass, typename FuncType>
	void BindAttributeChangeDelegate(UAbilitySystemComponent* ASC, FGameplayAttribute Attribute, UClass Class, FuncType Func);

	void OnHealthChanged(const FOnAttributeChangeData& Data);
	void OnMaxHealthChanged(const FOnAttributeChangeData& Data);
};

template<class UClass, typename FuncType>
inline void AITBattlePlayerController::BindAttributeChangeDelegate(UAbilitySystemComponent* ASC, FGameplayAttribute Attribute, UClass Class, FuncType Func)
{
	ASC->GetGameplayAttributeValueChangeDelegate(Attribute).AddUObject(Class, Func);
}
