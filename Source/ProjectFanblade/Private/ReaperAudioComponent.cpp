#include "ReaperAudioComponent.h"

#include "ReaperAudioComponent.h"

#include "Reaper.h"

#include "Math/Vector.h"
#include "Components/AudioComponent.h"

UReaperAudioComponent::UReaperAudioComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}

void UReaperAudioComponent::BeginPlay()
{
	Super::BeginPlay();
	Reaper = Cast<AReaper>(GetOwner());
	if(!Reaper){return;}
	PropAudioComponent = Reaper->GetPropAudioComponent();
	MotorAudioComponent = Reaper->GetMotorAudioComponent();
}

void UReaperAudioComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	float PropSpeed = EngineCommands[0].Throttle;
	float Airspeed = AircraftState.CalibratedAirSpeedKts;
	AActor* PlayerCharacter = GetWorld()->GetFirstPlayerController()->GetPawn();
	float DopplerShift{0.0};
	// Conversion factor from feet to cm
	const float FeetToCmFactor = 30.48; // :(
	if (PlayerCharacter)
	{
		FVector PlayerVelocity{};
		FVector PlayerPosition = PlayerCharacter->GetTransform().GetTranslation();
		AAircraft* AircraftPlayer = Cast<AAircraft>(PlayerCharacter);
		if (AircraftPlayer)
		{
			FVector VelocityNEDfps = AircraftPlayer->GetAircraftMovementComponent()->AircraftState.VelocityNEDfps;
			
			// Convert to cm/s
			PlayerVelocity = VelocityNEDfps * FeetToCmFactor;

		}
		else
		{
			PlayerVelocity = GetOwner()->GetVelocity();
		}

		
		FVector SourceVelocityNEDfps = Reaper->GetAircraftMovementComponent()->AircraftState.VelocityNEDfps;
		FVector SourceVelocity = SourceVelocityNEDfps * FeetToCmFactor;
		FVector SourcePosition = GetOwner()->GetActorLocation();
		
		const float SpeedOfSound = 34300.0f;

		
		DopplerShift = CalculateDopplerShift(SpeedOfSound,PlayerVelocity,PlayerPosition,SourceVelocity,SourcePosition);
		/** Log the vectors and the Doppler shift */
		UE_LOG(LogTemp, Warning, TEXT("SpeedOfSound: %f"), SpeedOfSound);
		UE_LOG(LogTemp, Warning, TEXT("PlayerVelocity: %s"), *PlayerVelocity.ToString());
		UE_LOG(LogTemp, Warning, TEXT("PlayerPosition: %s"), *PlayerPosition.ToString());
		UE_LOG(LogTemp, Warning, TEXT("SourceVelocity: %s"), *SourceVelocity.ToString());
		UE_LOG(LogTemp, Warning, TEXT("SourcePosition: %s"), *SourcePosition.ToString());
		UE_LOG(LogTemp, Warning, TEXT("DopplerShift: %f"), DopplerShift);

		
		UE_LOG(LogTemp, Warning, TEXT("DopplerShift: %F"), DopplerShift);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayerCharacter is null."));
	}
	
	if(!PropAudioComponent){return;}
	//for now i get the throttle since i know this returns a value, more setup of the flight model might be needed to get more acurate engine and prop info.
	PropAudioComponent->SetFloatParameter("PropSpeed",PropSpeed);
	PropAudioComponent->SetFloatParameter("Airspeed",Airspeed);
	PropAudioComponent->SetFloatParameter("DopplerShift",DopplerShift);
	
	
	

}