// Copyright Epic Games, Inc. All Rights Reserved.

#include "NSAudioDSPLibrary.h"
#include "MetasoundFrontendRegistries.h"

#define LOCTEXT_NAMESPACE "FNSAudioDSPLibraryModule"

void FNSAudioDSPLibraryModule::StartupModule()
{
    FMetasoundFrontendRegistryContainer::Get()->RegisterPendingNodes();
}

void FNSAudioDSPLibraryModule::ShutdownModule()
{
}

#undef LOCTEXT_NAMESPACE
    
IMPLEMENT_MODULE(FNSAudioDSPLibraryModule, NSAudioDSPLibrary)
