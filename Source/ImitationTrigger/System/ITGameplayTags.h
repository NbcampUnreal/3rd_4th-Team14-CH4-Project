#pragma once

#include "Containers/UnrealString.h"
#include "Containers/Map.h"
#include "GameplayTagContainer.h"

class UGameplayTagManager;

struct FITGameplayTags
{
	static const FITGameplayTags& Get() { return GameplayTags; }
	static void InitializeNativeTags();

	void AddTag(FGameplayTag& OutTag, const ANSICHAR* TagName, const ANSICHAR* TagComment);
	void AddAllTags(UGameplayTagsManager& TagsManager);

	/* 입력 태그 */
	FGameplayTag InputTag_Move;
	FGameplayTag InputTag_Look_Mouse;
	FGameplayTag InputTag_Crouch;

private:
	static FITGameplayTags GameplayTags;
};