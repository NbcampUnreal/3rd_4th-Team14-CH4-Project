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

	/* ìž…ë ¥ íƒœê·¸ */
	FGameplayTag InputTag_Move;
	FGameplayTag InputTag_Look_Mouse;
	FGameplayTag InputTag_Look_Aim;
	FGameplayTag InputTag_Crouch;

	/* Ability íƒœê·¸ */
	FGameplayTag Ability_InputBlocked;

	// ÄðÅ¸ÀÓ ÅÂ±×
	FGameplayTag Cooldown_Obstacle;

private:
	static FITGameplayTags GameplayTags;
};