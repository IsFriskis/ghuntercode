// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.IO;

public class TinyTavernGame : ModuleRules
{
	public TinyTavernGame(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        // Used with SDK
        if (Target.Platform == UnrealTargetPlatform.Win64)
        {
            string SteamVersion = "v160"; // Example Steam SDK version
            string SteamDir = "E:/UE_5.3/Engine/Source/ThirdParty/Steamworks/Steamv160/sdk"; // Modify with your SDK location
            PublicAdditionalLibraries.Add(Path.Combine(SteamDir, "redistributable_bin", "win64", "steam_api64.lib"));
            PublicIncludePaths.Add(Path.Combine(SteamDir, "public"));
            RuntimeDependencies.Add(Path.Combine(SteamDir, "redistributable_bin", "win64", "steam_api64.dll"));
        }

        PublicDependencyModuleNames.AddRange(new string[]
		{
			"ArticyRuntime",
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"EnhancedInput",
			"CommonUI",
			"CommonInput",
			"UMG",
			"LevelSequence",
			"MovieScene",
			"MoviePlayer",
			"AIModule",
			"GameplayTasks",
			"NavigationSystem",
			"Niagara",
			"Paper2D",
			"MotionWarping",
			"AdvancedWidgets",
			"GameplayAbilities", 
	        "GameplayTags", 
	        "GameplayTasks",
	        "MediaAssets",
            "OnlineSubsystem",
            "OnlineSubsystemSteam"
		});

		// Uncomment if you are using Slate UI
		PrivateDependencyModuleNames.AddRange(new string[]
		{
			"Navmesh",
			"AdvancedWidgets",
			"Slate",
			"SlateCore"
		});
		
		// Uncomment if you are using online features
		PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
