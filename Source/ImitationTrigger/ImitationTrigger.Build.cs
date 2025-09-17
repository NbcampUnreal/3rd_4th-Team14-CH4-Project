// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class ImitationTrigger : ModuleRules
{
	public ImitationTrigger(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicIncludePaths.AddRange(new string[] {
			"ImitationTrigger"
		});

		PublicDependencyModuleNames.AddRange(new string[] {
			// Base
			"Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput",
			"UMG",
			"Slate",
			"SlateCore",
			"OnlineSubsystem",
			"Networking",
			"Sockets",

			// Gameplay
			"GameplayTags",
			"GameplayTasks",
			"GameplayAbilities",

			// Network
			"NetCore"
		});

		PrivateDependencyModuleNames.AddRange(new string[] { 
			"InputCore",
			"ApplicationCore" });

	}
}
