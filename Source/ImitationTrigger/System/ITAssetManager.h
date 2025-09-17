#pragma once

#include "Engine/AssetManager.h"
#include "ITAssetManager.generated.h"

UCLASS()
class IMITATIONTRIGGER_API UITAssetManager : public UAssetManager
{
	GENERATED_BODY()

public:
	UITAssetManager();
	static UITAssetManager& Get(); // �̱���

private:
	virtual void StartInitialLoading() override;
};