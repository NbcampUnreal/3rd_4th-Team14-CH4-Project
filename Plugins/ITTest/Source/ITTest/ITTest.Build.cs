// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class ITTest : ModuleRules
{
	public ITTest(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicIncludePaths.AddRange(
			new string[] {
				// ... add public include paths required here ...
			}
			);
				
		
		PrivateIncludePaths.AddRange(
			new string[] {
				// ... add other private include paths required here ...
			}
			);
			
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput",
				"UMG",
				"Slate",
				"SlateCore",
				"OnlineSubsystem",
				"Networking",
				"Sockets",
				"ImitationTrigger",
			}
			);
			
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CoreUObject",
				"Engine",
				"Slate",
				"SlateCore",
				"InputCore",
				"ApplicationCore",
				"OnlineSubsystemSteam",
				"GameplayTags",
				"GameplayTasks",
				"GameplayAbilities",
			}
			);
		
		
		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
				// ... add any modules that your module loads dynamically here ...
			}
			);
	}
}