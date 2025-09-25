#pragma once

#include "Engine/DataAsset.h"
#include "AbilitySystem/Attributes/ITAttributeTableRow.h"
#include "Cosmetics/ITCharacterPartType.h"
#include "Cosmetics/ITCharacterAnimComponent.h"
#include "Camera/ITCameraMode.h"
#include "ITPawnData.generated.h"

class UITCameraMode;
class UITAbilitySet;
class UITInputConfig;
class UDataTable;

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

	// Character에 부여할(grand) Ability 정보
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PawnData|Abilities")
	TArray<TObjectPtr<UITAbilitySet>> AbilitySets;

	// Character의 입력 설정
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PawnData|Input")
	TObjectPtr<UITInputConfig> InputConfig;

	// Character에 적용할 Cosmetic(외형) 정보
	UPROPERTY(EditDefaultsOnly, Category = "PawnData|Cosmetics")
	TArray<FITCharacterPart> InitCharacterParts;

	UPROPERTY(EditDefaultsOnly, Category = "PawnData|Cosmetics")
	FITAnimBodyStyleSelectionSet InitBodyMeshes;

	UPROPERTY(EditDefaultsOnly, Category = "PawnData|Cosmetics")
	TArray<FITAnimLayerEntry> AnimLayerRules;

	// Character에 적용할 기본 카메라 모드
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PawnData|Camera")
	TSubclassOf<UITCameraMode> DefaultCameraMode;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PawnData|Camera")
	TArray<FCameraModeWithTag> CameraModeRules;

	// Character의 Attribute Set 초기값
	UPROPERTY(EditDefaultsOnly, Category = "PawnData|Attributes", meta = (RowType = "ITAttributeTableRow"))
	TObjectPtr<UDataTable> InitDataTable;
};
