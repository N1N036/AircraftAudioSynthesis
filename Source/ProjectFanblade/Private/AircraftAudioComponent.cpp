// Copyright (c) 2023-present Nino Saglia & Tim Verberne. All rights reserved.
// Written by Tim Verberne.

#include "AircraftAudioComponent.h"

#include "JSBSimMovementComponent.h"
#include "GameFramework/Actor.h"
#include "GameFramework/MovementComponent.h"

DEFINE_LOG_CATEGORY_CLASS(UAircraftAudioComponent, LogAircraftAudio)

UAircraftAudioComponent::UAircraftAudioComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UAircraftAudioComponent::BeginPlay()
{
	Super::BeginPlay();

	/** Get and cast the movement component from the parent actor */
	JSBSimMovementComp = Cast<UJSBSimMovementComponent>(GetOwner()->FindComponentByClass<UJSBSimMovementComponent>());}

void UAircraftAudioComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (!JSBSimMovementComp){return;}

	//Data to be used later in specific components to drive audio parameters.
	AircraftState = JSBSimMovementComp->AircraftState;
	EngineStates = JSBSimMovementComp->EngineStates;
	EngineCommands = JSBSimMovementComp->EngineCommands;
	Gears = JSBSimMovementComp->Gears;
	
	
}


float UAircraftAudioComponent::CalculateDopplerShift(float speedOfSound, 
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