#pragma once

#include "Engine/DataAsset.h"
#include "Cosmetics/ITCharacterPartType.h"
#include "ITPawnData.generated.h"

class UITAbilitySet;
class UITInputConfig;
class UITCameraMode;

/**
 * Pawn을 정의하기 위한 속성(properties)를 가지고 있는 Data Asset
 * ULyraPawnData의 구성을 참고함
 */
UCLASS(BlueprintType, Const)
class IMITATIONTRIGGER_API UITPawnData : public UDataAsset
{
	GENERATED_BODY()

public:
	UITPawnData(const FObjectInitializer& ObjectInitializer);

	// Pawn 또는 Character에 부여할(grand) Ability 정보
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PawnData|Abilities")
	TArray<TObjectPtr<UITAbilitySet>> AbilitySets;

	// Pawn 또는 Character의 입력 설정
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PawnData|Input")
	TObjectPtr<UITInputConfig> InputConfig;

	// Pawn 또는 Character에 적용할 Cosmetic(외형) 정보
	UPROPERTY(EditAnywhere, Category = "PawnData|Cosmetics")
	TArray<FITCharacterPart> InitCharacterParts;

	UPROPERTY(EditAnywhere, Category = "PawnData|Cosmetics")
	FITAnimBodyStyleSelectionSet InitBodyMeshes;

	// Pawn 또는 Character에 적용할 기본 카메라 모드
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PawnData|Camera")
	TSubclassOf<UITCameraMode> DefaultCameraMode;
};
