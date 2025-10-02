#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ITLobbyWidget.generated.h"

class UButton;
class UTextBlock;

UCLASS()
class IMITATIONTRIGGER_API UITLobbyWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	// 매칭 상태 업데이트 (PlayerController에서 호출)
	UFUNCTION(BlueprintCallable)
	void UpdateMatchmakingState(bool bIsInQueue);

protected:
	// 로비 화면 전용 위젯들
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite)
	UButton* StartButton;

	UPROPERTY(meta = (BindWidget), BlueprintReadWrite)
	UTextBlock* StartButtonText;

	UPROPERTY(meta = (BindWidget), BlueprintReadWrite)
	UButton* ExitButton;

	UPROPERTY(meta = (BindWidget), BlueprintReadWrite)
	UButton* Character1;

	UPROPERTY(meta = (BindWidget), BlueprintReadWrite)
	UButton* Character2;

	UPROPERTY(meta = (BindWidget), BlueprintReadWrite)
	UButton* Character3;

	UFUNCTION()
	void OnStartClicked();

	UFUNCTION()
	void OnExitClicked();

private:
	// 현재 매칭 상태
	bool bIsInMatchmakingQueue = false;

	// UI 업데이트
	void UpdateStartButtonAppearance();
};
