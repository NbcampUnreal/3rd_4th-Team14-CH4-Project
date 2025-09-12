// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerBarWidget.generated.h"

class UProgressBar;
class UTextBlock;
class UImage;
/**
 * 
 */
UCLASS()
class IMITATIONTRIGGER_API UPlayerBarWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "PlayerBar")
	void UpdateHealthBar(float CurrentHealth, float MaxHealth);

	UFUNCTION(BlueprintCallable, Category = "PlayerBar")
	void UpdateShieldBar(float CurrentShield, float MaxShield);


protected:

	virtual void NativePreConstruct() override;
	
	virtual void NativeConstruct() override;

	UPROPERTY(meta = (BindWidget))
	UImage* CharacterImage;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* PlayerNameText;

	UPROPERTY(meta = (BindWidget))
	UProgressBar* HealthBar;

	UPROPERTY(meta = (BindWidget))
	UProgressBar* ShieldBar;

private:

	UPROPERTY(visibleAnywhere, Category = "PlayerBar")
	float HealthPercent;

	UPROPERTY(visibleAnywhere, Category = "PlayerBar")
	float ShieldPercent;

	UPROPERTY(editanywhere, Category = "PlayerBar")
	FText NameText;


	
};
