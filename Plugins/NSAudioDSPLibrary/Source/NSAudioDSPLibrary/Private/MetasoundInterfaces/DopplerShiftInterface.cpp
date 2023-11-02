#include "MetasoundInterfaces/DopplerShiftInterface.h"

#include "AudioDevice.h"
#include "UObject/AnimPhysObjectVersion.h"
#include "UObject/FortniteMainBranchObjectVersion.h"



UDopplerShiftInterface::UDopplerShiftInterface(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// constructor
}


float UDopplerShiftInterface::GetDopplerPitchMultiplier(float& CurrentPitchScale, bool bSmooth, FListener const& InListener, const FVector Location, const FVector Velocity, float DeltaTime)
{
	static const float SpeedOfSoundInAirAtSeaLevel = 33000.f;		// cm/sec

	FVector const SourceToListenerNorm = (InListener.Transform.GetTranslation() - Location).GetSafeNormal();

	// find source and listener speeds along the line between them
	float const SourceVelMagTorwardListener = Velocity | SourceToListenerNorm;
	float const ListenerVelMagAwayFromSource = InListener.Velocity | SourceToListenerNorm;

	// multiplier = 1 / (1 - ((sourcevel - listenervel) / speedofsound) );
	float const InvDopplerPitchScale = 1.f - ( (SourceVelMagTorwardListener - ListenerVelMagAwayFromSource) / SpeedOfSoundInAirAtSeaLevel );
	float const PitchScale = 1.f / InvDopplerPitchScale;

	// factor in user-specified intensity
	float const FinalPitchScale = ((PitchScale - 1.f)) + 1.f;


	UE_LOG(LogAudio, Warning, TEXT("Applying doppler pitchscale %f, raw scale %f, deltaspeed was %f"), FinalPitchScale, PitchScale, ListenerVelMagAwayFromSource - SourceVelMagTorwardListener);
		return FinalPitchScale;
}

#define LOCTEXT_NAMESPACE "AudioParameterInterface"
#define AUDIO_PARAMETER_INTERFACE_NAMESPACE "NS.DopplerShift"
namespace Audio
{
	namespace DopplerShiftInterface
	{
		const FName Name = AUDIO_PARAMETER_INTERFACE_NAMESPACE;

		namespace Inputs
		{
			const FName DopplerShift = AUDIO_PARAMETER_INTERFACE_MEMBER_DEFINE("DopplerShift");
		}

		Audio::FParameterInterfacePtr GetInterface()
		{
			struct FInterface : public Audio::FParameterInterface
			{
				FInterface()
					: FParameterInterface(DopplerShiftInterface::Name, { 1, 0 })
				{
					Inputs =
					{
						{
							FText(),
							NSLOCTEXT("AudioGeneratorInterface_DopplerShift", "DopplerDescription", "Doppler shift as heard from the listeners perspective."),
							FName(),
							{ Inputs::DopplerShift, 1.0f }
						}
					};
				}
			};

			static FParameterInterfacePtr InterfacePtr;
			if (!InterfacePtr.IsValid())
			{
				InterfacePtr = MakeShared<FInterface>();

			}
			return InterfacePtr;
		}
	}
}
#undef AUDIO_PARAMETER_INTERFACE_NAMESPACE