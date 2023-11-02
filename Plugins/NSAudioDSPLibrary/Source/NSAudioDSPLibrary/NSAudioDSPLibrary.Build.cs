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
                "MetasoundGraphCore",
                "MetasoundFrontend",
                "AudioExtensions",
                "Serialization",
                "SignalProcessing",
                "CoreUObject",
                "Engine",
                "Projects",

    
            }
            );
            
        
        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                "Core",
                "CoreUObject",
                "Engine",
                "AudioMixer",
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
