
#pragma once
#include "MetasoundInterfaces/DopplerShiftInterface.h"


/**
 * 
 */
class NSAudioPluginListener : public IAudioPluginListener
{
public:
    NSAudioPluginListener();

    //~ Begin IAudioPluginListener
    virtual void OnListenerInitialize(FAudioDevice* AudioDevice, UWorld* ListenerWorld) override;
    virtual void OnListenerUpdated(FAudioDevice* AudioDevice, const int32 ViewportIndex, const FTransform& ListenerTransform, const float InDeltaSeconds) override;
    virtual void OnListenerShutdown(FAudioDevice* AudioDevice) override;
    virtual void OnWorldChanged(FAudioDevice* AudioDevice, UWorld* ListenerWorld) override;
    //~ End IAudioPluginListener

protected:
	TMap<FActiveSound*, FVector> LastSoundLocationMap; //For calculating the velocity of each tracked sound.
	FVector LastListenerLocation; //For calculating the velocity of the listener.

	public:
	float GetDopplerPitchMultiplier(FVector InLastListenerLocation, FVector InListenerLocation, FVector InLastSourceLocation, FVector InSourceLocation, float InDeltaSeconds);

private:
    // Connection to the base plugin module, where we keep track of the audio devices that spawn us
    class FNSAudioDSPLibraryModule* NSAudioDSPLibraryModule;

    Audio::FParameterInterfacePtr DopplerShiftInterface = Audio::DopplerShiftInterface::GetInterface();
};