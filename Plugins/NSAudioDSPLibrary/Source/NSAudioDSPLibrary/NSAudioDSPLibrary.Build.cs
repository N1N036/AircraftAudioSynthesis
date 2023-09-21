// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class NSAudioDSPLibrary : ModuleRules
{
    public NSAudioDSPLibrary(ReadOnlyTargetRules Target) : base(Target)
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
                "Core",
                "MetasoundFrontend",
                "MetasoundGraphCore",
            }
            );
            
        
        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                "CoreUObject",
                "Engine",
                "SignalProcessing",
            }
            );
        
        
        DynamicallyLoadedModuleNames.AddRange(
            new string[]
            {

            }
            );
    }
}
