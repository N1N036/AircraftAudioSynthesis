// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "NSAudioSourceDataOverride.h"
#include "Modules/ModuleManager.h"



class FNSAudioDSPLibraryModule : public IModuleInterface
{
public:
    
    
    
    void RegisterAudioDevice(unsigned AudioDeviceId);
    void UnregisterAudioDevice(unsigned AudioDeviceId);
    void RegisterAudioDevice(FAudioDevice* AudioDeviceHandle);
    void UnregisterAudioDevice(FAudioDevice* AudioDeviceHandle);



    IAudioPluginFactory* GetPluginFactory(EAudioPlugin PluginType);

    
    virtual void StartupModule() override;

    virtual void ShutdownModule() override;
    
    

private:

    // List of registered audio devices.
    TArray<FAudioDevice*> RegisteredAudioDevices;

};
