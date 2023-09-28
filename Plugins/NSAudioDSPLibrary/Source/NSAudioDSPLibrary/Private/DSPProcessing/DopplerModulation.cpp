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
    }

    void FDopplerModulation::SetParameters(
        float InDelayFeedback,
        float InModulationFeedback,
        float InDelayTimeSeconds,
        bool InInvertModulationSignal)
    {
        DelayFeedback = InDelayFeedback;
        float ModulationFeedback = InModulationFeedback; 
        DelayTimeSeconds = InDelayTimeSeconds;
        bool InvertModulationSignal = InInvertModulationSignal;

        /** Invert the modulation feedback coefficient based on the boolean. */
        InvertedModulationFeedback = InvertModulationSignal ? -ModulationFeedback : ModulationFeedback;

        UpdateDelay();
    }

    void FDopplerModulation::UpdateDelay() {
        DelayBuffer.SetEasedDelayMsec(DelayTimeSeconds * 1000);
    }




    void FDopplerModulation::ProcessAudioBuffer(const float* InBuffer, const float* InModulation, float* OutBuffer, int32 NumSamples)
    {
        float DelayLength = DelayBuffer.GetDelayLengthSamples();


        for (int32 FrameIndex = 0; FrameIndex < NumSamples; ++FrameIndex)
        {

            /** Write sample with feedback into delay buffer and increment */
            DelayBuffer.WriteDelayAndInc(InBuffer[FrameIndex] + DelayFeedback * FeedbackSample);

            /** Compute modulated delay length with feedback. */
            float ModulatedDelayLength = DelayLength - (DelayLength * (InModulation[FrameIndex] + InvertedModulationFeedback * FeedbackSample));
            ModulatedDelayLength = FMath::Clamp(ModulatedDelayLength, 0, DelayLength); //for safety.
            /** Write the modulated sample to output buffer. */
            OutBuffer[FrameIndex] = DelayBuffer.ReadDelayAt(ModulatedDelayLength);

            FeedbackSample = DelayBuffer.ReadDelayAt(ModulatedDelayLength);
        }
    }
}
