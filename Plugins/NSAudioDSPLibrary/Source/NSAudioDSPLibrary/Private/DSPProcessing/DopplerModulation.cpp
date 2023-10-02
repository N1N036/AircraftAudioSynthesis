#include "DSPProcessing/DopplerModulation.h"
#include "DSP/Delay.h"




namespace DSPProcessing
{

    FDopplerModulation::FDopplerModulation()
    {
    }


    void FDopplerModulation::Init(float InSampleRate, float DelayTimeMax)
    {

        DelayBuffer.Init(InSampleRate, DelayTimeMax);
        DelayBuffer.SetDelayMsec(DelayTimeMax);
        DelayBuffer.SetEaseFactor(0.1f);
    }

    void FDopplerModulation::SetParameters(
        float InDelayFeedback,
        float InModulationFeedback,
        float InDelayTimeSeconds,
        bool InInvertModulationSignal,
        float InMaxSlope)
    {
        DelayFeedback = InDelayFeedback;
        ModulationFeedback = InModulationFeedback; 
        DelayTimeSeconds = InDelayTimeSeconds;
        ModulationSignalInverter = (InInvertModulationSignal ? 1 : -1) ;
        MaxSlope = InMaxSlope;

        UpdateDelay();
    }

    void FDopplerModulation::UpdateDelay() {
        DelayBuffer.SetEasedDelayMsec(DelayTimeSeconds);
    }




    void FDopplerModulation::ProcessAudioBuffer(const float* InBuffer, const float* InModulation, float* OutBuffer, int32 NumSamples)
    {
        float DelayLength = DelayBuffer.GetDelayLengthSamples();
        float MaxSlopeDSP = MaxSlope / DelayLength;
       
        /** DSP Calculations. */
            for (int32 FrameIndex = 0; FrameIndex < NumSamples; ++FrameIndex)
            {
                DelayBuffer.WriteDelayAndInc(InBuffer[FrameIndex] + DelayFeedback * FeedbackSample);
                float RawModulationSignal = 0.5f * DelayLength + 0.5f * ModulationSignalInverter * DelayLength * (InModulation[FrameIndex] + ModulationFeedback * FeedbackSample);

                // Low-pass filter the modulation signal
                float FilteredModulationSignal {MaxSlope * RawModulationSignal + (1 - MaxSlope) * PrevModulationSignal};
                PrevModulationSignal = FilteredModulationSignal;

                float ModulatedDelayLength = DelayLength - FilteredModulationSignal;
                ModulatedDelayLength = FMath::Clamp(ModulatedDelayLength, 0, DelayLength);

                OutBuffer[FrameIndex] = DelayBuffer.ReadDelayAt(ModulatedDelayLength);
                FeedbackSample = DelayBuffer.ReadDelayAt(ModulatedDelayLength);
            }
    }
}
