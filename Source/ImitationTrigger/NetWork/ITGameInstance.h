// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "ITGameInstance.generated.h"


UCLASS()
class IMITATIONTRIGGER_API UITGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	// 선택된 캐릭터 인덱스 저장
	UPROPERTY(BlueprintReadWrite, Category = "Character")
	int32 SelectedCharacterIndex = 0;

	UFUNCTION(BlueprintCallable, Category = "Character")
	void SetSelectedCharacterIndex(int32 Index) { SelectedCharacterIndex = Index; }

	UFUNCTION(BlueprintCallable, Category = "Character")
	int32 GetSelectedCharacterIndex() const { return SelectedCharacterIndex; }

	UPROPERTY(BlueprintReadWrite, Category = "Player")
	FString PlayerNickname = TEXT("Player");

	UFUNCTION(BlueprintCallable, Category = "Player")
	void SetPlayerNickname(const FString& Nickname);

	UFUNCTION(BlueprintCallable, Category = "Player")
	FString GetPlayerNickname() const { return PlayerNickname; }
	
};
