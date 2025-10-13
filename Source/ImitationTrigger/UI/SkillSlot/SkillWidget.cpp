// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/SkillSlot/SkillWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

void USkillWidget::SetSkillImage(UTexture2D* NewImage)
{
	if (SkillImage)
	{
		if (SkillImage)
		{
			FSlateBrush Brush = SkillImage->GetBrush();
			Brush.SetResourceObject(NewImage);
			SkillImage->SetBrush(Brush);
		}
	}
}

void USkillWidget::OnSkill(float CoolDown)
{
	if (CoolDownBlock)
	{
		OnTickStartTime = UGameplayStatics::GetTimeSeconds(this);
		CurrentCoolDownTime = CoolDown;

		UpdateCoolDownText();
		CoolDownBlock->SetVisibility(ESlateVisibility::Visible);
		KeyImage->SetVisibility(ESlateVisibility::Collapsed);
		SkillImage->SetRenderOpacity(0.5f);

		UWorld* World = GetWorld();
		if (World)
		{
			World->GetTimerManager().SetTimer(
				CoolDownTimerHandle,
				this,
				&USkillWidget::CoolDownTimerTick,
				0.1f,
				true
				);
		}
		
	}
}

void USkillWidget::NativeConstruct()
{

	UWorld* World = GetWorld();
	if (World)
	{
		World->GetTimerManager().ClearTimer(CoolDownTimerHandle);
	}
	Super::NativeConstruct();
}

void USkillWidget::CoolDownTimerTick()
{
	float CurrentTime = UGameplayStatics::GetTimeSeconds(this);
	float DeltaTime = CurrentTime - OnTickStartTime;
	
	CurrentCoolDownTime -= DeltaTime;
	OnTickStartTime = UGameplayStatics::GetTimeSeconds(this);

	if (CurrentCoolDownTime <= 0.0f)
	{
		CurrentCoolDownTime = 0.0f;

		UWorld* World = GetWorld();
		if (World)
		{
			World->GetTimerManager().ClearTimer(CoolDownTimerHandle);
		}

		if (CoolDownBlock)
		{
			CoolDownBlock->SetVisibility(ESlateVisibility::Collapsed);
			KeyImage->SetVisibility(ESlateVisibility::Visible);
			SkillImage->SetRenderOpacity(1.0f);
		}
	}
	UpdateCoolDownText();
}

void USkillWidget::UpdateCoolDownText()
{
	if (CoolDownBlock)
	{
		FString Time = FString::Printf(TEXT("%.1f 초"), CurrentCoolDownTime);
		CoolDownBlock->SetText(FText::FromString(Time));
	}
}
