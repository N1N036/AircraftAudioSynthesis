#pragma once

#include "CoreMinimal.h"
#include "DSPProcessing/Helpers/ParamSmoother.h"
#include "DSP/Delay.h"


namespace DSPProcessing
{
	class FDopplerModulation
	{
	public:
		FDopplerModulation();

		void SetSampleRate(float SampleRateIn);

		void InitDelayFeedbackParamSmoothing(float SmoothingTimeInMs, float SampleRate);
		void InitModulationFeedbackParamSmoothing(float SmoothingTimeInMs, float SampleRate);
		void InitModulationLowPassParamSmoothing(float SmoothingTimeInMs, float SampleRate);
		void InitModulationHighPassParamSmoothing(float SmoothingTimeInMs, float SampleRate);
		void InitDelayBuffer(float DelayTimeMax, float SmoothingFactor, float InSampleRate);
		
		void SetDelayFeedback(float InDelayFeedback);
		void SetModulationFeedback(float InModulationFeedback);
		void SetDelayTimeSeconds(float InDelayTimeSeconds);
		void SetInvertModulationSignal(bool InInVertModulationSignal);
		void SetModulationLowPass(float InLowPass);
		void SetModulationHighPass(float InHighPass);

		float CalculateFilterAlpha(float SampleRate, float CutoffFrequency);
		
		void ProcessAudioBuffer(const float* InBuffer,const float* InModulation, float* OutBuffer, int32 NumSamples);

		
	private:
		float SampleRate;
		bool InvertModulationSignal;

		ParamSmootherLPF DelayFeedbackParamSmootherLPF;
		ParamSmootherLPF ModulationFeedbackParamSmootherLPF;
		ParamSmootherLPF ModulationLowPassParamSmootherLPF;
		ParamSmootherLPF ModulationHighPassParamSmootherLPF;
		
		Audio::FDelay DelayBuffer;
		
		float FeedbackSample{0.0f};
		float PrevModulationSignal{0.0f};
		float PrevHighPassModulationSignal{0.0f};
		
	};
}
