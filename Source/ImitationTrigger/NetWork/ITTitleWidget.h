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
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite)
	UEditableTextBox* ServerIPEdit;
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite)
	UButton* PlayButton;
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite)
	UButton* ExitButton;
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite)
	UButton* StartButton;



	UFUNCTION()
	void OnPlayClicked();

	UFUNCTION()
	void OnExitClicked();

	UFUNCTION()
	void OnStartClicked();


};
