// Copyright (c) 2023-present Nino Saglia & Tim Verberne. All rights reserved.
// Written by Tim Verberne.

#include "AircraftAudioController.h"

#include "Aircraft.h"


class AAircraft;
DEFINE_LOG_CATEGORY_CLASS(UAircraftAudioController, LogAircraftAudio)

UAircraftAudioController::UAircraftAudioController()
{
	PrimaryComponentTick.bCanEverTick = true;
	SetComponentTickEnabled(true);
}

void UAircraftAudioController::BeginPlay()
{
	Super::BeginPlay();
}

void UAircraftAudioController::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	AActor* PlayerCharacter = GetWorld()->GetFirstPlayerController()->GetPawn();
	AAircraft* OwnerAircraft = Cast<AAircraft>(GetOwner());

	//DEPRECATED
	//As the SJBSimMovement component doesn't inherit from a movement component GetVelocity() returns 0.
	//Therefore we need to get the velocity from the flight dynamic model and convert from feet per second to meter per second.
	const float FeetToCmFactor = 30.48; 

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

			if(!OwnerAircraft){return;}
			FVector SourceVelocityNEDfps = OwnerAircraft->GetAircraftMovementComponent()->AircraftState.VelocityNEDfps;
			FVector SourceVelocity = SourceVelocityNEDfps * FeetToCmFactor;
			FVector SourcePosition = GetOwner()->GetActorLocation();
		
			const float SpeedOfSound = 34300.0f;

			AirSpeed = SourceVelocityNEDfps.Length();
			DopplerShift = CalculateDopplerShift(SpeedOfSound,PlayerVelocity,PlayerPosition,SourceVelocity,SourcePosition);
	}
}


const float UAircraftAudioController::CalculateDopplerShift(float speedOfSound, 
							const FVector& observerVelocity, const FVector& observerPosition, 
							const FVector& sourceVelocity, const FVector& sourcePosition)
{

	FVector const SourceToListenerNorm = (observerPosition - sourcePosition).GetSafeNormal();
	// find source and listener speeds along the line between them
	float const SourceVelMagTorwardListener = sourceVelocity | SourceToListenerNorm;
	float const ListenerVelMagAwayFromSource = observerVelocity | SourceToListenerNorm;

	// multiplier = 1 / (1 - ((sourcevel - listenervel) / speedofsound) );
	float const InvDopplerPitchScale = 1.f - ( (SourceVelMagTorwardListener - ListenerVelMagAwayFromSource) / speedOfSound );
	float const PitchScale = 1.f / InvDopplerPitchScale;
	
	{
		return PitchScale;
	}
}

