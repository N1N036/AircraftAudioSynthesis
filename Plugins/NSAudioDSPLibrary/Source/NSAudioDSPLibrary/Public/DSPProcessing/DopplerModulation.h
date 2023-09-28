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

		void Init(float InSampleRate, float DelayTimeMax);

		void SetParameters(
			float InDryLevel,
			float InWetLevel,
			float InFeedback,
			float InMaxDelayTimeSeconds);

		
		void ProcessAudioBuffer(const float* InBuffer,const float* InModulation, float* OutBuffer, int32 NumSamples);

	private:
		
		float DelayTime;
		float DryLevel;
		float WetLevel;
		float Feedback;

		// The internal delay buffer
		Audio::FDelay DelayBuffer;
		
		float FeedbackSample{0.f};

		float MaxDelayTimeSeconds{0.0f};


	};
}
