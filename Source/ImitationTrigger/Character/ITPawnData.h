#pragma once

#include "Engine/DataAsset.h"
#include "ITPawnData.generated.h"

class UITAbilitySet;
class UITInputConfig;
class UITCameraMode;

/**
 * Pawn�� �����ϱ� ���� �Ӽ�(properties)�� ������ �ִ� Data Asset
 * ULyraPawnData�� ������ ������
 */
UCLASS(BlueprintType, Const)
class IMITATIONTRIGGER_API UITPawnData : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UITPawnData(const FObjectInitializer& ObjectInitializer);

	// Pawn�̳� Character�� �ο���(grand) Ability ����
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PawnData|Abilities")
	TArray<TObjectPtr<UITAbilitySet>> AbilitySets;

	// Pawn�̳� Character�� �ο���(grand) Ability ����
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PawnData|Abilities")
	TObjectPtr<UITInputConfig> InputConfig;

	// Pawn�̳� Character�� �ο���(grand) Ability ����
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PawnData|Abilities")
	TSubclassOf<UITCameraMode> DefaultCameraMode;
};
