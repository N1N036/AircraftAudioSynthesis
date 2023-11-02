

#include "NSAudioDSPLibrary.h"
#include "AudioDeviceManager.h"
#include "MetasoundFrontendRegistries.h"
#include "NSAudioPluginListener.h"
#include "MetasoundInterfaces/DopplerShiftInterface.h"
#include "Engine/Engine.h"


#define LOCTEXT_NAMESPACE "FNSAudioDSPLibraryModule"
class IAudioModulationManager;


/**
 * The NS AudioDSPLibrary is a collection of various DSP effects intended to extend unreal's audio features.
 * The primary focus is audio DSP effects but i will make audio systems as needed and won't keep features limited to this.
 */


void FNSAudioDSPLibraryModule::StartupModule()
{
	FAudioDeviceManagerDelegates::OnAudioDeviceCreated.AddRaw(this, &FNSAudioDSPLibraryModule::RegisterAudioDevice);
	FAudioDeviceManagerDelegates::OnAudioDeviceDestroyed.AddRaw(this, &FNSAudioDSPLibraryModule::UnregisterAudioDevice);
	
	
	FMetasoundFrontendRegistryContainer::Get()->RegisterPendingNodes();
	Audio::IAudioParameterInterfaceRegistry& InterfaceRegistry = Audio::IAudioParameterInterfaceRegistry::Get();
	InterfaceRegistry.RegisterInterface(Audio::DopplerShiftInterface::GetInterface());
}

void FNSAudioDSPLibraryModule::ShutdownModule()
{
	IModuleInterface::ShutdownModule();
}


FAudioDevice* GetAudioDeviceFromID(uint32 DeviceID)
{
	if (GEngine && GEngine->GetAudioDeviceManager())
	{
		FAudioDeviceHandle AudioDevicePtr = GEngine->GetAudioDeviceManager()->GetAudioDevice(DeviceID);
		return AudioDevicePtr.GetAudioDevice();
	}
	return nullptr;
}


void FNSAudioDSPLibraryModule::RegisterAudioDevice(unsigned AudioDeviceId)
{
	if(!AudioDeviceId){return;}
	FAudioDevice* AudioDeviceHandle = GetAudioDeviceFromID(AudioDeviceId);
	RegisterAudioDevice(AudioDeviceHandle);
}

void FNSAudioDSPLibraryModule::UnregisterAudioDevice(unsigned AudioDeviceId)
{
	if(!AudioDeviceId){return;}
	FAudioDevice* AudioDeviceHandle = GetAudioDeviceFromID(AudioDeviceId);
	UnregisterAudioDevice(AudioDeviceHandle);
}

void FNSAudioDSPLibraryModule::RegisterAudioDevice(FAudioDevice* AudioDeviceHandle)
{
	if(!AudioDeviceHandle){return;}
	if (!RegisteredAudioDevices.Contains(AudioDeviceHandle))
	{
		TAudioPluginListenerPtr ContextManager = TAudioPluginListenerPtr(new (NSAudioPluginListener));
		AudioDeviceHandle->RegisterPluginListener(ContextManager);
		RegisteredAudioDevices.Add(AudioDeviceHandle);
	}
}

void FNSAudioDSPLibraryModule::UnregisterAudioDevice(FAudioDevice* AudioDeviceHandle)
{
	if(!AudioDeviceHandle){return;}
	RegisteredAudioDevices.Remove(AudioDeviceHandle);
}

#undef LOCTEXT_NAMESPACE
    
IMPLEMENT_MODULE(FNSAudioDSPLibraryModule, NSAudioDSPLibrary)
