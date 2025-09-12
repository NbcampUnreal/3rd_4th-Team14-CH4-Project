// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/ComboBoxString.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"
#include "NetworkTest/ITMatchmakingTypes.h"
#include "ITMatchmakingWidget.generated.h"

class UITGameInstance;
class UITMatchmakingSubsystem;

UCLASS()
class ITTEST_API UITMatchmakingWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
    virtual void NativeConstruct() override;
    virtual void NativeDestruct() override;

protected:
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
    UButton* ITMatchmakingButton;

    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
    UButton* ITCancelButton;

    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
    UComboBoxString* ITGameModeComboBox;

    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
    UTextBlock* ITStatusText;

    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
    UTextBlock* ITQueuePositionText;

    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
    UTextBlock* ITEstimatedTimeText;

    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
    UProgressBar* ITMatchmakingProgress;

    UFUNCTION()
    void ITOnMatchmakingButtonClicked();

    UFUNCTION()
    void ITOnCancelButtonClicked();

    UFUNCTION()
    void ITOnGameModeChanged(FString SelectedItem, ESelectInfo::Type SelectionType);

    UFUNCTION()
    void ITUpdateMatchmakingStatus(FString Status);

    UFUNCTION()
    void ITOnMatchFound(FString SessionID, FString ServerAddress);

    UFUNCTION(BlueprintImplementableEvent, Category = "IT UI Events")
    void ITOnMatchmakingStarted();

    UFUNCTION(BlueprintImplementableEvent, Category = "IT UI Events")
    void ITOnMatchmakingCancelled();

    UFUNCTION(BlueprintImplementableEvent, Category = "IT UI Events")
    void ITOnMatchFoundEvent(const FString& SessionID);

private:
    FTimerHandle ITUpdateTimerHandle;
    FDateTime ITMatchmakingStartTime;

    UFUNCTION()
    void ITUpdateQueueInfo();

    EITMatchmakingMode ITGetSelectedGameMode();
    void ITSetUIState(bool bIsMatchmaking);
    void ITInitializeGameModeComboBox();
};
