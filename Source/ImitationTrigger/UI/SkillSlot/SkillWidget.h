// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SkillWidget.generated.h"

class UTextBlock;
class UImage;
/**
 * 
 */
UCLASS()
class IMITATIONTRIGGER_API USkillWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category = "UI")
	void SetSkillImage(UTexture2D* NewImage);

	UFUNCTION(BlueprintCallable, Category = "UI")
	void OnSkill(float CoolDown);


protected:

	virtual void NativeConstruct() override;

	UPROPERTY(meta = (BindWidget))
	UImage* SkillImage;

	UPROPERTY(meta = (BindWidget))
	UImage* KeyImage;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* CoolDownBlock;

private:

	float CurrentCoolDownTime;

	FTimerHandle CoolDownTimerHandle;
	

	void CoolDownTimerTick();

	void UpdateCoolDownText();
	

	float OnTickStartTime = 0.0f;
};
