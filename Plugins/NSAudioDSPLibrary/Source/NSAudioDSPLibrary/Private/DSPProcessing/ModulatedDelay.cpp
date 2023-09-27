#include "DSPProcessing/ModulatedDelay.h"
#include "DSP/Delay.h"




namespace DSPProcessing
{

    FModulatedDelay::FModulatedDelay()
    {

    }


    void FModulatedDelay::Init(float InSampleRate, float DelayTimeMax)
    {

        DelayBuffer.Init(InSampleRate, DelayTimeMax);
        DelayBuffer.SetDelayMsec(DelayTimeMax);
    }

    void FModulatedDelay::SetParameters(float InDryLevel, float InWetLevel, float InFeedback, float InMaxDelayTime)
    {
        DryLevel = InDryLevel;
        WetLevel = InWetLevel;
        Feedback = InFeedback;
        MaxDelayTimeSeconds = InMaxDelayTime;
    }




    void FModulatedDelay::ProcessAudioBuffer(const float* InBuffer, const float* InModulation, float* OutBuffer, int32 NumSamples)
    {
        float DelayLength = DelayBuffer.GetDelayLengthSamples();
        float FeedbackCoefficient = 0.5f;  // Adjust between 0.0 to 1.0 as needed

        for (int32 FrameIndex = 0; FrameIndex < NumSamples; ++FrameIndex)
        {

            // Write sample with feedback into delay buffer and increment
            DelayBuffer.WriteDelayAndInc(InBuffer[FrameIndex]);
            // Compute modulated delay length
            float ModulatedDelayLength = DelayLength - (DelayLength * (InModulation[FrameIndex] + .5 * FeedbackSample));

          

            // Write the modulated sample to output buffer
            OutBuffer[FrameIndex] = DelayBuffer.ReadDelayAt(ModulatedDelayLength);

            // Apply feedback by scaling the output and summing it with the input
            FeedbackSample = DelayBuffer.ReadDelayAt(ModulatedDelayLength);
        }
    }
}
