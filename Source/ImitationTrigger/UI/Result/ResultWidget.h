// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ResultWidget.generated.h"

class UImage;
class UTextBlock;
class UButton;
/**
 * 
 */
UCLASS()
class IMITATIONTRIGGER_API UResultWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category = "UI")
	void SetResult(int32 TotalPlayers, int32 MyRank, UTexture2D* CharacterImage, FText PlayerName, int32 SurvivalTime, int32 KillCount, float Damage);

	virtual void NativeConstruct() override;

	// 승자 이름 설정
	void SetWinnerName(const FString& WinnerName);

protected:

	UPROPERTY(meta = (BindWidget))
	UTextBlock* PlayerRankBlock;

	UPROPERTY(meta = (BindWidget))
	UImage* PlayerImage;
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock* PlayerNameBlock;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* SurvivalTimeBlock;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* KillCountBlock;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* DamageBlock;

	// 승자 이름 텍스트 (블루프린트에서 바인딩)
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> WinnerText;

	// 로비로 돌아가기 버튼 (블루프린트에서 바인딩)
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> ReturnToLobbyButton;

	// 버튼 클릭 이벤트
	UFUNCTION()
	void OnReturnToLobbyClicked();

private:
	// 현재 서버 IP 추출
	FString GetLobbyServerAddress();

	
};
