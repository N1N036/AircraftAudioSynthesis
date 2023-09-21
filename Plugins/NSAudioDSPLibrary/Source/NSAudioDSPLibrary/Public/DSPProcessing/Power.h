#pragma once

#include "CoreMinimal.h"


namespace DSPProcessing
{
	class FPower
	{
	public:
		FPower();
		void Init();
		void SetPower(const float InAmount);
		void ProcessAudioBuffer(const float* InBuffer, float* OutBuffer, int32 NumSamples);

	private:
		float Power;
	};
}
