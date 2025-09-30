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
	FGameplayTag InputTag_Look_Aim;
	FGameplayTag InputTag_Crouch;

	/* Ability 태그 */
	FGameplayTag Ability_InputBlocked;

	// ��Ÿ�� �±�
	FGameplayTag Cooldown_Obstacle;

private:
	static FITGameplayTags GameplayTags;
};