#include "DSPProcessing/DopplerModulation.h"
#include "DSP/Delay.h"
#include "DSP/Filter.h"


namespace DSPProcessing
{
    FDopplerModulation::FDopplerModulation()
    {
    }
    
    void FDopplerModulation::SetSampleRate(float InSampleRate)
    {
        SampleRate = InSampleRate;
    }
    

    void FDopplerModulation::InitDelayFeedbackParamSmoothing(float SmoothingTimeInMs, float InSampleRate)
    {
        DelayFeedbackParamSmootherLPF.Init(SmoothingTimeInMs, InSampleRate);
       
    }

    void FDopplerModulation::InitModulationFeedbackParamSmoothing(float SmoothingTimeInMs, float InSampleRate)
    {
        ModulationFeedbackParamSmootherLPF.Init(SmoothingTimeInMs, InSampleRate);
    }
    
    
    void FDopplerModulation::InitModulationLowPassParamSmoothing(float SmoothingTimeInMs, float InSampleRate)
    {
        ModulationLowPassParamSmootherLPF.Init(SmoothingTimeInMs, InSampleRate);
    }
    
    void FDopplerModulation::InitModulationHighPassParamSmoothing(float SmoothingTimeInMs, float InSampleRate)
    {
        ModulationHighPassParamSmootherLPF.Init(SmoothingTimeInMs, InSampleRate);
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

    void FDopplerModulation::SetModulationLowPass(float InLowPass)
    {
        float LowPass = CalculateFilterAlpha(SampleRate,InLowPass);
        ModulationLowPassParamSmootherLPF.SetNewParamValue(LowPass);
    }

    void FDopplerModulation::SetModulationHighPass(float InHighPass)
    {
        float HighPass = CalculateFilterAlpha(SampleRate,InHighPass);
        ModulationHighPassParamSmootherLPF.SetNewParamValue(HighPass);
    }

    
    float FDopplerModulation::CalculateFilterAlpha(float InSampleRate, float CutoffFrequency)
    {
        float Tan = FMath::Tan(PI * CutoffFrequency / InSampleRate);
        return ( Tan - 1 ) / ( Tan + 1 );
    }



    void FDopplerModulation::ProcessAudioBuffer(const float* InBuffer, const float* InModulation, float* OutBuffer, int32 NumSamples)
    {
        float LowPassAlpha = ModulationLowPassParamSmootherLPF.GetValue();
        float HighPassaAlpha = ModulationHighPassParamSmootherLPF.GetValue();

        float ModulationFeedbackStrength = ModulationFeedbackParamSmootherLPF.GetValue();
        float DelayFeedbackStrength = DelayFeedbackParamSmootherLPF.GetValue();
        float DelayLength = DelayBuffer.GetDelayLengthSamples();
        
        /** DSP Calculations. */
        for (int32 FrameIndex = 0; FrameIndex < NumSamples; ++FrameIndex)
        {

            DelayBuffer.WriteDelayAndInc(InBuffer[FrameIndex] + DelayFeedbackStrength * FeedbackSample);
            float RawModulationSignal = 0.5f * DelayLength + 0.5f * 1 * DelayLength * (InModulation[FrameIndex] + ModulationFeedbackStrength * FeedbackSample);

            // Low-pass filter the modulation signal
            float FilteredModulationSignal = (RawModulationSignal + LowPassAlpha * PrevModulationSignal - HighPassaAlpha * PrevModulationSignal)/3;
            PrevModulationSignal = FilteredModulationSignal;
            
            float ModulatedDelayLength = DelayLength - FilteredModulationSignal;
            ModulatedDelayLength = FMath::Clamp(ModulatedDelayLength, 0.0f, DelayLength);

            OutBuffer[FrameIndex] = DelayBuffer.ReadDelayAt(ModulatedDelayLength);
            FeedbackSample = DelayBuffer.ReadDelayAt(ModulatedDelayLength);

        }
    }
}
