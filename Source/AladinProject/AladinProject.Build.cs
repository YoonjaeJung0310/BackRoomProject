// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class AladinProject : ModuleRules
{
	public AladinProject(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core",
            "CoreUObject", 
            "Engine", 
            "InputCore", 
            "EnhancedInput",
            "UMG",
            "MediaAssets"
        });

        PrivateDependencyModuleNames.AddRange(new string[] { });

        
        
    }
}
