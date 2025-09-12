// Fill out your copyright notice in the Description page of Project Settings.


#include "NetworkTest/ITMatchmakingWidget.h"
#include "Components/Button.h"
#include "Components/ComboBoxString.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"
#include "NetworkTest/ITGameInstance.h"
#include "NetworkTest/ITMatchmakingSubsystem.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "Misc/DateTime.h"
#include "Misc/Timespan.h" 

void UITMatchmakingWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (ITMatchmakingButton)
    {
        ITMatchmakingButton->OnClicked.AddDynamic(this, &UITMatchmakingWidget::ITOnMatchmakingButtonClicked);
    }

    if (ITCancelButton)
    {
        ITCancelButton->OnClicked.AddDynamic(this, &UITMatchmakingWidget::ITOnCancelButtonClicked);
    }

    if (ITGameModeComboBox)
    {
        ITGameModeComboBox->OnSelectionChanged.AddDynamic(this, &UITMatchmakingWidget::ITOnGameModeChanged);
        ITInitializeGameModeComboBox();
    }

    if (UITGameInstance* GameInstance = Cast<UITGameInstance>(UGameplayStatics::GetGameInstance(this)))
    {
        if (UITMatchmakingSubsystem* MatchmakingSystem = GameInstance->GetSubsystem<UITMatchmakingSubsystem>())
        {
            MatchmakingSystem->OnITMatchFound.AddDynamic(this, &UITMatchmakingWidget::ITOnMatchFound);
            MatchmakingSystem->OnITMatchmakingStatusChanged.AddDynamic(this, &UITMatchmakingWidget::ITUpdateMatchmakingStatus);
        }
    }

    ITSetUIState(false);
}

void UITMatchmakingWidget::NativeDestruct()
{
    if (GetWorld())
    {
        GetWorld()->GetTimerManager().ClearTimer(ITUpdateTimerHandle);
    }

    Super::NativeDestruct();
}

void UITMatchmakingWidget::ITOnMatchmakingButtonClicked()
{
    if (UITGameInstance* GameInstance = Cast<UITGameInstance>(UGameplayStatics::GetGameInstance(this)))
    {
        if (GameInstance->ITIsCurrentlyMatchmaking())
        {
            return;
        }

        EITMatchmakingMode SelectedMode = ITGetSelectedGameMode();
        GameInstance->ITStartMatchmaking(SelectedMode);

        ITSetUIState(true);
        ITMatchmakingStartTime = FDateTime::Now();

        GetWorld()->GetTimerManager().SetTimer(
            ITUpdateTimerHandle,
            this,
            &UITMatchmakingWidget::ITUpdateQueueInfo,
            1.0f,
            true
        );

        ITOnMatchmakingStarted();

        UE_LOG(LogTemp, Log, TEXT("Matchmaking started for mode: %d"), (int32)SelectedMode);
    }
}

void UITMatchmakingWidget::ITOnCancelButtonClicked()
{
    if (UITGameInstance* GameInstance = Cast<UITGameInstance>(UGameplayStatics::GetGameInstance(this)))
    {
        GameInstance->ITStopMatchmaking();

        ITSetUIState(false);

        if (GetWorld())
        {
            GetWorld()->GetTimerManager().ClearTimer(ITUpdateTimerHandle);
        }

        ITOnMatchmakingCancelled();

        UE_LOG(LogTemp, Log, TEXT("Matchmaking cancelled"));

        ITOnMatchmakingCancelled();

        UE_LOG(LogTemp, Log, TEXT("Matchmaking cancelled"));
    }
}

void UITMatchmakingWidget::ITOnGameModeChanged(FString SelectedItem, ESelectInfo::Type SelectionType)
{
    if (SelectionType == ESelectInfo::Direct)
    {
        return;
    }

    UE_LOG(LogTemp, Log, TEXT("Game mode changed to: %s"), *SelectedItem);
}

void UITMatchmakingWidget::ITUpdateMatchmakingStatus(FString Status)
{
    UE_LOG(LogTemp, Warning, TEXT("UI Widget received status: %s"), *Status);

    if (ITStatusText)
    {
        // 큐 업데이트 메시지 파싱
        if (Status.StartsWith(TEXT("QUEUE|")))
        {
            UE_LOG(LogTemp, Warning, TEXT("Parsing QUEUE message: %s"), *Status);
            TArray<FString> Parts;
            Status.ParseIntoArray(Parts, TEXT("|"));

            if (Parts.Num() == 3)
            {
                int32 Mode = FCString::Atoi(*Parts[1]);
                int32 QueueCount = FCString::Atoi(*Parts[2]);

                // 현재 매칭 모드와 같은 경우만 UI 업데이트
                if (UITGameInstance* GameInstance = Cast<UITGameInstance>(UGameplayStatics::GetGameInstance(this)))
                {
                    if ((int32)GameInstance->ITCurrentMatchMode == Mode)
                    {
                        if (ITQueuePositionText)
                        {
                            FString PositionText = FString::Printf(TEXT("Waiting Users: %d"), QueueCount);
                            ITQueuePositionText->SetText(FText::FromString(PositionText));
                        }
                    }
                }
                return;
            }
            // 일반 상태 메시지 표시
            ITStatusText->SetText(FText::FromString(Status));
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("ITStatusText is null!")); // 추가
        }

        // 일반 상태 메시지 표시
        ITStatusText->SetText(FText::FromString(Status));
    }
}

void UITMatchmakingWidget::ITOnMatchFound(FString SessionID, FString ServerAddress)
{
    ITSetUIState(false);

    if (ITStatusText)
    {
        ITStatusText->SetText(FText::FromString(TEXT("Match Found! Game Connect...")));
    }

    ITOnMatchFoundEvent(SessionID);

    UE_LOG(LogTemp, Log, TEXT("Match found UI update: %s"), *SessionID);
}

void UITMatchmakingWidget::ITUpdateQueueInfo()
{
    if (UITGameInstance* GameInstance = Cast<UITGameInstance>(UGameplayStatics::GetGameInstance(this)))
    {
        if (!GameInstance->ITIsCurrentlyMatchmaking())
        {
            GetWorld()->GetTimerManager().ClearTimer(ITUpdateTimerHandle);
            return;
        }

        // 서버의 실제 큐 정보를 사용하지 말고, 로컬 타이머만 표시
        FTimespan ElapsedTime = FDateTime::Now() - ITMatchmakingStartTime;

        if (ITEstimatedTimeText)
        {
            float TimeUntilTimeout = 20.0f - ElapsedTime.GetTotalSeconds();
            if (TimeUntilTimeout > 0)
            {
                FString TimeoutText = FString::Printf(TEXT("Auto Start until: %.0f seconds"), TimeUntilTimeout);
                ITEstimatedTimeText->SetText(FText::FromString(TimeoutText));
            }
            else
            {
                FString TimeText = FString::Printf(TEXT("Waiting Time: %02d:%02d"),
                    (int32)ElapsedTime.GetMinutes(),
                    (int32)ElapsedTime.GetSeconds() % 60);
                ITEstimatedTimeText->SetText(FText::FromString(TimeText));
            }
        }
    }

}

EITMatchmakingMode UITMatchmakingWidget::ITGetSelectedGameMode()
{
    if (ITGameModeComboBox)
    {
        FString SelectedText = ITGameModeComboBox->GetSelectedOption();

        if (SelectedText == TEXT("Solo"))
            return EITMatchmakingMode::Solo;
        else if (SelectedText == TEXT("Duo"))
            return EITMatchmakingMode::Duo;
        else if (SelectedText == TEXT("Trio"))
            return EITMatchmakingMode::Trio;
        else if (SelectedText == TEXT("Squad"))
            return EITMatchmakingMode::Squad;
    }

    return EITMatchmakingMode::Solo;
}

void UITMatchmakingWidget::ITSetUIState(bool bIsMatchmaking)
{
    if (ITMatchmakingButton)
    {
        ITMatchmakingButton->SetIsEnabled(!bIsMatchmaking);
    }

    if (ITCancelButton)
    {
        ITCancelButton->SetIsEnabled(bIsMatchmaking);
        ITCancelButton->SetVisibility(bIsMatchmaking ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
    }

    if (ITGameModeComboBox)
    {
        ITGameModeComboBox->SetIsEnabled(!bIsMatchmaking);
    }

    if (ITMatchmakingProgress)
    {
        ITMatchmakingProgress->SetVisibility(bIsMatchmaking ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
        if (bIsMatchmaking)
        {
            ITMatchmakingProgress->SetPercent(0.0f);
        }
    }

    if (ITQueuePositionText)
    {
        ITQueuePositionText->SetVisibility(bIsMatchmaking ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
    }

    if (ITEstimatedTimeText)
    {
        ITEstimatedTimeText->SetVisibility(bIsMatchmaking ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
    }
}

void UITMatchmakingWidget::ITInitializeGameModeComboBox()
{
    if (ITGameModeComboBox)
    {
        ITGameModeComboBox->ClearOptions();
        ITGameModeComboBox->AddOption(TEXT("Solo"));
        ITGameModeComboBox->AddOption(TEXT("Duo"));
        ITGameModeComboBox->AddOption(TEXT("Trio"));
        ITGameModeComboBox->AddOption(TEXT("Squad"));
        ITGameModeComboBox->SetSelectedOption(TEXT("Solo"));
    }
}