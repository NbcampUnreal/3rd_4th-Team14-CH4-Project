// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class ITTest : ModuleRules
{
    public ITTest(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicIncludePaths.AddRange(
            new string[] {

            }
            );


        PrivateIncludePaths.AddRange(
            new string[] {

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
                "Sockets"
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
                "OnlineSubsystemSteam"
				// ... add private dependencies that you statically link with here ...	
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
