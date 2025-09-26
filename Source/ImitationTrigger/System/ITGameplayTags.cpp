#include "ITGameplayTags.h"
#include "GameplayTagsManager.h"

FITGameplayTags FITGameplayTags::GameplayTags;

void FITGameplayTags::InitializeNativeTags()
{
	UGameplayTagsManager& TagsManager = UGameplayTagsManager::Get();
	GameplayTags.AddAllTags(TagsManager);
}

void FITGameplayTags::AddTag(FGameplayTag& OutTag, const ANSICHAR* TagName, const ANSICHAR* TagComment)
{
	UGameplayTagsManager& TagsManager = UGameplayTagsManager::Get();
	FName Tag = FName(TagName);
	FString Comment = FString(TEXT("(Native) ")) + FString(TagComment);
	OutTag = TagsManager.AddNativeGameplayTag(Tag, Comment);
}

void FITGameplayTags::AddAllTags(UGameplayTagsManager& TagsManager)
{
	AddTag(InputTag_Move, "InputTag.Move", "Move");
	AddTag(InputTag_Look_Mouse, "InputTag.Look.Mouse", "Look using Mouse");
	AddTag(InputTag_Look_Aim, "InputTag.Look.Aim", "Look using Aim");
	AddTag(InputTag_Crouch, "InputTag.Crouch", "Crouch");


	AddTag(Ability_InputBlocked, "Ability.InputBlocked", "Blocks Input");

	AddTag(Cooldown_Obstacle, "Cooldown.Obstacle", "Cooldown tag for obstacle spawn");

}
