#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ITTitleWidget.generated.h"

class UEditableTextBox;
class UButton;

UCLASS()
class IMITATIONTRIGGER_API UITTitleWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

protected:
	// 타이틀 화면 전용 위젯들
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite)
	UEditableTextBox* ServerIPEdit;

	UPROPERTY(meta = (BindWidget), BlueprintReadWrite)
	UButton* JoinServerButton;  // PlayButton → JoinServerButton으로 변경

	UFUNCTION()
	void OnJoinServerClicked();  // OnPlayClicked → OnJoinServerClicked로 변경
};
