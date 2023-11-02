// Copyright (c) 2022 Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.
#include "NSAudioPluginListener.h"
#include "AudioDevice.h"
#include "NSAudioDSPLibrary.h"

NSAudioPluginListener::NSAudioPluginListener()
{
}

void NSAudioPluginListener::OnListenerInitialize(FAudioDevice* AudioDevice, UWorld* ListenerWorld)
{
    // Only initialize if this is a game playing. Either a real game or play in editor session
    if (ListenerWorld == nullptr || (ListenerWorld->WorldType != EWorldType::Game && ListenerWorld->WorldType != EWorldType::PIE))
    {
        return;
    }

    if (!NSAudioDSPLibraryModule)
    {
        NSAudioDSPLibraryModule = &FModuleManager::GetModuleChecked<FNSAudioDSPLibraryModule>("NSAudioDSPLibrary");
    }
}

//Calculates doppler shift.
//TODO; this code will be refactored to implement each interface calculation in a decoupled manner like in ActivceSound.cpp
//TODO; found a bug where the doppler value shoots up when a sonic boom event gets created from a different actor.
void NSAudioPluginListener::OnListenerUpdated(FAudioDevice* AudioDevice, const int32 ViewportIndex, const FTransform& ListenerTransform, const float InDeltaSeconds)
{
    if (NSAudioDSPLibraryModule == nullptr){return;}

    FVector ListenerLocation = ListenerTransform.GetLocation();

    TArray<FActiveSound*> ActiveSounds = AudioDevice->GetActiveSounds();
    for(FActiveSound* ActiveSound : ActiveSounds)
    {
        bool bImplementsDopplerInterface = ActiveSound->GetSound()->ImplementsParameterInterface(DopplerShiftInterface);
        if(!bImplementsDopplerInterface){continue;}  // Changed from return to continue
        if(!ActiveSound->bLocationDefined){continue;} // Changed from return to continue
        
        FVector LastSoundLocation;
        FVector SoundLocation = ActiveSound->LastLocation;
        float DopplerPitchToApply = 1;
        
        if (LastSoundLocationMap.Contains(ActiveSound))
        {
            LastSoundLocation = LastSoundLocationMap[ActiveSound];
            DopplerPitchToApply = GetDopplerPitchMultiplier(LastListenerLocation,ListenerLocation,LastSoundLocation,SoundLocation,InDeltaSeconds);
            LastSoundLocationMap[ActiveSound] = SoundLocation;
        }
        else
        {
            LastSoundLocationMap.Add(ActiveSound, SoundLocation);
        }
        auto Transmitter = ActiveSound->GetTransmitter();
        TArray<FAudioParameter> ParamsToUpdate;
        ParamsToUpdate.Add({ Audio::DopplerShiftInterface::Inputs::DopplerShift, DopplerPitchToApply });
            
        Transmitter->SetParameters(MoveTemp(ParamsToUpdate));
    }
    
    LastListenerLocation =  ListenerLocation;

    TArray<FActiveSound*> SoundsInMap;
    LastSoundLocationMap.GetKeys(SoundsInMap);

    for (int32 i = SoundsInMap.Num() - 1; i >= 0; --i)
    {
        FActiveSound* Sound = SoundsInMap[i];
        if (Sound->bFinished)
        {
            LastSoundLocationMap.Remove(Sound); // Correctly remove the finished sound from the map
        }
    }
}



float NSAudioPluginListener::GetDopplerPitchMultiplier(FVector InLastListenerLocation, FVector InListenerLocation, FVector InLastSourceLocation, FVector InSourceLocation, float InDeltaSeconds)
{
    static const float SpeedOfSoundCmPerSec = 33000.f;
    
    FVector ListenerVelocity = (InListenerLocation - InLastListenerLocation) / InDeltaSeconds;
    FVector SourceVelocity = (InSourceLocation - InLastSourceLocation) / InDeltaSeconds;

    // Calculate the velocity of the listener and source relative to the direction of the source
    FVector SourceToListener = InListenerLocation - InSourceLocation;
    SourceToListener.Normalize();

    float ListenerVelocityInDirection = FVector::DotProduct(ListenerVelocity, SourceToListener);
    float SourceVelocityInDirection  = FVector::DotProduct(SourceVelocity, SourceToListener);

    // Calculate the pitch scale
    float DifferenceInPitch = (SourceVelocityInDirection - ListenerVelocityInDirection) / SpeedOfSoundCmPerSec;
    float PitchMultiplier = 1 + DifferenceInPitch;
    float const ClampedPitchMultiplier = FMath::Clamp(PitchMultiplier, UE_SMALL_NUMBER,2.0f);
    return ClampedPitchMultiplier;
}



void NSAudioPluginListener::OnWorldChanged(FAudioDevice* AudioDevice, UWorld* ListenerWorld)
{
    OnListenerInitialize(AudioDevice, ListenerWorld);
}

void NSAudioPluginListener::OnListenerShutdown(FAudioDevice* AudioDevice)
{
    if (NSAudioDSPLibraryModule){return;}
    
    NSAudioDSPLibraryModule->UnregisterAudioDevice(AudioDevice);
}