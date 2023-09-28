#pragma once

#include "CoreMinimal.h"
#include "MetasoundEnumRegistrationMacro.h"
#include "MetasoundParamHelper.h"
#include "DSP/Delay.h"
#include "MetasoundBuilderInterface.h"

namespace DSPProcessing
{
	class FDopplerModulation
	{
	public:
		FDopplerModulation();

		void Init(float InSampleRate, float DelayTime);

		void SetParameters(
			float InDelayFeedback,
			float InModulationFeedback,
			float InDelayTimeSeconds,
			bool InInvertModulationSignal);

		void UpdateDelay();

		
		void ProcessAudioBuffer(const float* InBuffer,const float* InModulation, float* OutBuffer, int32 NumSamples);

	private:

		float DelayFeedback;
		//float ModulationFeedback; local variable
		float DelayTimeSeconds;
		//bool InvertModulationSignal; local variable

		float InvertedModulationFeedback;

		// The internal delay buffer
		Audio::FDelay DelayBuffer;
		
		float FeedbackSample{0.f};


	};
}
