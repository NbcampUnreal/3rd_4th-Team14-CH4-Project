// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Item/ITItemDefinition.h"
#include "ITItemFragmentBPFL.generated.h"

// 어빌리티를 구현하기 위한 블루프린트용 함수 추가
UCLASS()
class IMITATIONTRIGGER_API UITItemFragmentBPFL : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category = "Item|Fragment")
	static bool GetFragment_Grenade(const UITItemDefinition* ItemDef, FITItemFragment_Grenade& Value);
};
