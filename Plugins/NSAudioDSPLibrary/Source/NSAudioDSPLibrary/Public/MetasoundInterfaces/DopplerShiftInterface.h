#pragma once

#include "AudioDevice.h"
#include "IAudioParameterInterfaceRegistry.h"
#include "AudioLinkSettingsAbstract.h"
#include "DopplerShiftInterface.generated.h"

UCLASS(BlueprintType, hidecategories=Object, editinlinenew, MinimalAPI)
class UDopplerShiftInterface : public UObject
{
	GENERATED_UCLASS_BODY()
	
public:
	float GetDopplerPitchMultiplier(float& CurrentPitchScale, bool bSmooth, FListener const& InListener,
	                                FVector Location,
	                                FVector Velocity, float DeltaTime);
};
namespace Audio
{
	namespace DopplerShiftInterface
	{
		NSAUDIODSPLIBRARY_API const extern FName Name;

		namespace Inputs
		{
			NSAUDIODSPLIBRARY_API const extern FName DopplerShift;
		} // namespace Inputs

		NSAUDIODSPLIBRARY_API Audio::FParameterInterfacePtr GetInterface();
	};
}; 
