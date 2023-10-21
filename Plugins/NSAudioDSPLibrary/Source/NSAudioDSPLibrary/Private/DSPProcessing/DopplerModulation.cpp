#include "DSPProcessing/DopplerModulation.h"
#include "DSP/Delay.h"




namespace DSPProcessing
{

    FDopplerModulation::FDopplerModulation()
    {
    }
    

    void FDopplerModulation::InitDelayFeedbackParamSmoothing(float SmoothingTimeInMs, float SampleRate)
    {
        DelayFeedbackParamSmootherLPF.Init(SmoothingTimeInMs, SampleRate);
       
    }

    void FDopplerModulation::InitModulationFeedbackParamSmoothing(float SmoothingTimeInMs, float SampleRate)
    {
        ModulationFeedbackParamSmootherLPF.Init(SmoothingTimeInMs, SampleRate);
    }
    
    
    void FDopplerModulation::InitMaxSlopeParamSmoothing(float SmoothingTimeInMs, float SampleRate)
    {
        MaxSlopeParamSmootherLPF.Init(SmoothingTimeInMs, SampleRate);
    }

    void FDopplerModulation::InitDelayBuffer(float DelayTimeMax,float SmoothingFactor, float InSampleRate)
    {

        float ClampedSmoothingFactor = FMath::Clamp(SmoothingFactor,0.0f,1.0f);
         
        DelayBuffer.Init(InSampleRate, 1);
        DelayBuffer.SetEaseFactor(ClampedSmoothingFactor); //SetEaseFactor references EaseDelayMsec, Msec this isn't correct since its a Factor and only aceepts <0,1>
        DelayBuffer.SetDelayMsec(1);

    }

    void FDopplerModulation::SetDelayFeedback(float InDelayFeedback)
    {
        DelayFeedbackParamSmootherLPF.SetNewParamValue(InDelayFeedback);
    }

    void FDopplerModulation::SetModulationFeedback(float InModulationFeedack)
    {
        ModulationFeedbackParamSmootherLPF.SetNewParamValue(InModulationFeedack);
    }

    void FDopplerModulation::SetDelayTimeSeconds(float InDelayTimeMSec)
    {
        DelayBuffer.SetEasedDelayMsec(InDelayTimeMSec);
    }

    void FDopplerModulation::SetInvertModulationSignal(bool InInvertModulationSignal)
    {
        InvertModulationSignal = InInvertModulationSignal;
    }

    void FDopplerModulation::SetMaxSlope(float InMaxSlope)
    {
        MaxSlopeParamSmootherLPF.SetNewParamValue(InMaxSlope);
    }






    void FDopplerModulation::ProcessAudioBuffer(const float* InBuffer, const float* InModulation, float* OutBuffer, int32 NumSamples)
    {
        float CurrentMaxSlope = MaxSlopeParamSmootherLPF.GetValue();
        float ModulationFeedbackStrength = ModulationFeedbackParamSmootherLPF.GetValue();
        float DelayFeedbackStrength = DelayFeedbackParamSmootherLPF.GetValue();
        float DelayLength = DelayBuffer.GetDelayLengthSamples();
        
        /** DSP Calculations. */
        for (int32 FrameIndex = 0; FrameIndex < NumSamples; ++FrameIndex)
        {
            DelayBuffer.WriteDelayAndInc(InBuffer[FrameIndex] + DelayFeedbackStrength * FeedbackSample);
            float RawModulationSignal = 0.5f * DelayLength + 0.5f * 1 * DelayLength * (InModulation[FrameIndex] + ModulationFeedbackStrength * FeedbackSample);

            // Low-pass filter the modulation signal
            float FilteredModulationSignal {CurrentMaxSlope * RawModulationSignal + (1 - CurrentMaxSlope) * PrevModulationSignal};
            PrevModulationSignal = FilteredModulationSignal;

            float ModulatedDelayLength = DelayLength - FilteredModulationSignal;
            ModulatedDelayLength = FMath::Clamp(ModulatedDelayLength, 0.0f, DelayLength);

            OutBuffer[FrameIndex] = DelayBuffer.ReadDelayAt(ModulatedDelayLength);
            FeedbackSample = DelayBuffer.ReadDelayAt(ModulatedDelayLength);
        }
    }

    void FDopplerModulation::ProcessDopplerModulation(const float* InBuffer, const float* InModulation, float* OutBuffer, int32 NumSamples)
    {
        float CurrentMaxSlope = DelayFeedbackParamSmootherLPF.GetValue();
        float ModulationFeedbackStrength = ModulationFeedbackParamSmootherLPF.GetValue();
        float DelayFeedbackStrength = DelayFeedbackParamSmootherLPF.GetValue();
        float DelayLength = DelayBuffer.GetDelayLengthSamples();
        
        /** DSP Calculations. */
        for (int32 FrameIndex = 0; FrameIndex < NumSamples; ++FrameIndex)
        {
            DelayBuffer.WriteDelayAndInc(InBuffer[FrameIndex] + DelayFeedbackStrength * FeedbackSample);
            float RawModulationSignal = 0.5f * DelayLength + 0.5f * 1 * DelayLength * (InModulation[FrameIndex] + ModulationFeedbackStrength * FeedbackSample);

            // Low-pass filter the modulation signal
            float FilteredModulationSignal {CurrentMaxSlope * RawModulationSignal + (1 - CurrentMaxSlope) * PrevModulationSignal};
            PrevModulationSignal = FilteredModulationSignal;

            float ModulatedDelayLength = DelayLength - FilteredModulationSignal;
            ModulatedDelayLength = FMath::Clamp(ModulatedDelayLength, 0, DelayLength);

            OutBuffer[FrameIndex] = DelayBuffer.ReadDelayAt(ModulatedDelayLength);
            FeedbackSample = DelayBuffer.ReadDelayAt(ModulatedDelayLength);
        }
    }
}
