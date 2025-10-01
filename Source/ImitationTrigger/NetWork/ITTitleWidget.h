#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ITTitleWidget.generated.h"

class UEditableTextBox;
class UButton;
class UTextBlock;

UCLASS()
class IMITATIONTRIGGER_API UITTitleWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	// 매칭 상태 업데이트 (PlayerController에서 호출)
	UFUNCTION(BlueprintCallable)
	void UpdateMatchmakingState(bool bIsInQueue);

	// 현재 매칭 상태
	UPROPERTY(BlueprintReadOnly)
	bool bIsInMatchmakingQueue = false;

protected:
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite)
	UEditableTextBox* ServerIPEdit;
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite)
	UButton* PlayButton;
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite)
	UButton* ExitButton;
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite)
	UButton* StartButton;

	UPROPERTY(meta = (BindWidget), BlueprintReadWrite)
	UTextBlock* StartButtonText;



	UFUNCTION()
	void OnPlayClicked();

	UFUNCTION()
	void OnExitClicked();

	UFUNCTION()
	void OnStartClicked();

private:

	// UI 업데이트
	void UpdateStartButtonAppearance();

};
