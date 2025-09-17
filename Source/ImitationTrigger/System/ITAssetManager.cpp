#include "System/ITAssetManager.h"
#include "System/ITGameplayTags.h"

UITAssetManager::UITAssetManager()
{
}

UITAssetManager& UITAssetManager::Get()
{
	check(GEngine);
	UITAssetManager* ITAssetManager = Cast<UITAssetManager>(GEngine->AssetManager);
	return *ITAssetManager;
}
void UITAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();

	const FITGameplayTags& ITGameplayTag = FITGameplayTags::Get();
	ITGameplayTag.InitializeNativeTags();
}