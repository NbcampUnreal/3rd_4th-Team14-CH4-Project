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

	// Pawn �Ǵ� Character�� �ο���(grand) Ability ����
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PawnData|Abilities")
	TArray<TObjectPtr<UITAbilitySet>> AbilitySets;

	// Pawn �Ǵ� Character�� �Է� ����
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PawnData|Input")
	TObjectPtr<UITInputConfig> InputConfig;

	// Pawn �Ǵ� Character�� ������ �⺻ ī�޶� ���
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PawnData|Camera")
	TSubclassOf<UITCameraMode> DefaultCameraMode;
};
