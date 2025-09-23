// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/KillNotify/KillNotifyWidget.h"

#include "Components/TextBlock.h"

void UKillNotifyWidget::SetNotify(FText KillPlayer, FText DiePlayer)
{
	if (KillNotifyText)
	{
		FText NotifyText = FText::Format(
			NSLOCTEXT("KillNotify", "KillMessage", "{0}이(가) {1} 처치"),
			KillPlayer,
			DiePlayer
			);
		KillNotifyText->SetText(NotifyText);
	}
}
