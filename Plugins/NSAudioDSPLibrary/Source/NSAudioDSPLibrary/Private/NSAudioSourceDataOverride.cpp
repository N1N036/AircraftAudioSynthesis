
#include "NSAudioSourceDataOverride.h"
#include "Engine/World.h"

#include "Components/AudioComponent.h"
#include "MetasoundInterfaces/DopplerShiftInterface.h"


// Called during the Update call in MixerSource for each source
void FNSAudioSourceDataOverride::GetSourceDataOverrides(
    const uint32 SourceId, const FTransform& InListenerTransform, FWaveInstance* InOutWaveInstance)
{
    
    // Get source and listener location
    auto sourceLocation = InOutWaveInstance->Location;
    auto listenerLocation = InListenerTransform.GetLocation();

    
    // Grab the audio component belonging to this sound source
    auto audioComponentId = InOutWaveInstance->ActiveSound->GetAudioComponentID();
    auto audioComponent = UAudioComponent::GetAudioComponentFromID(audioComponentId);
    
    
    Audio::FParameterInterfacePtr paInterface = Audio::DopplerShiftInterface::GetInterface();
    TArray<FAudioParameter> paramsToUpdate;
    
    const bool isMetaSound = InOutWaveInstance->ActiveSound->GetSound()->ImplementsParameterInterface(paInterface);
    if (isMetaSound)
    {
        float DopplerValue = 3.34567f; //TODO: this arbitrary number is for testing purposes, later on the calculation needs to be made. 

        paramsToUpdate.Add({Audio::DopplerShiftInterface::Inputs::DopplerShift, DopplerValue});
        
        // Send the parameters to the MetaSound interface
        auto paramTransmitter = InOutWaveInstance->ActiveSound->GetTransmitter();
        if (paramTransmitter != nullptr)
        {
            paramTransmitter->SetParameters(MoveTemp(paramsToUpdate));
        }
    }
}









