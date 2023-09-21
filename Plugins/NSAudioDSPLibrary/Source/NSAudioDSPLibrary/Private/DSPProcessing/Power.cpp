#include "DSPProcessing/Power.h"


namespace DSPProcessing
{
	void FPower::Init()
	{
	}

	void FPower::SetPower(const float InAmount)
	{
		Power = InAmount;
	}

	void FPower::ProcessAudioBuffer(const float* InBuffer, float* OutBuffer, int32 NumSamples)
	{
		for (int32 i = 0; i < NumSamples; ++i)
		{
			OutBuffer[i] = FMath::Pow(InBuffer[i], Power);
		}
	}
}
