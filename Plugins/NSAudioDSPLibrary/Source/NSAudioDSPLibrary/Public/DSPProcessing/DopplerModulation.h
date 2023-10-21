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

		void InitDelayFeedbackParamSmoothing(float SmoothingTimeInMs, float SampleRate);
		void InitModulationFeedbackParamSmoothing(float SmoothingTimeInMs, float SampleRate);
		void InitMaxSlopeParamSmoothing(float SmoothingTimeInMs, float SampleRate);
		void InitDelayBuffer(float DelayTimeMax, float SmoothingFactor, float InSampleRate);
		
		void SetDelayFeedback(float InDelayFeedback);
		void SetModulationFeedback(float InModulationFeedback);
		void SetDelayTimeSeconds(float InDelayTimeSeconds);
		void SetInvertModulationSignal(bool InInVertModulationSignal);
		void SetMaxSlope(float InMaxSlope);


		
		void ProcessAudioBuffer(const float* InBuffer,const float* InModulation, float* OutBuffer, int32 NumSamples);

		
	private:
		
		void ProcessDopplerModulation(const float* InBuffer, const float* InModulation, float* OutBuffer, int32 NumSamples);

		bool InvertModulationSignal;

		ParamSmootherLPF DelayFeedbackParamSmootherLPF;
		ParamSmootherLPF ModulationFeedbackParamSmootherLPF;
		ParamSmootherLPF MaxSlopeParamSmootherLPF;
		
		Audio::FDelay DelayBuffer;
		
		float FeedbackSample{0.0f};
		float PrevModulationSignal{0.0f};



	};
}
