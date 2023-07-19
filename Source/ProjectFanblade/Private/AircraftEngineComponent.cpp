// Copyright (c) 2023-present Nino Saglia & Tim Verberne. All rights reserved.
// Written by Tim Verberne.

#include "AircraftEngineComponent.h"

DEFINE_LOG_CATEGORY_CLASS(UAircraftEngineComponent, LogAircraftEngine)

UAircraftEngineComponent::UAircraftEngineComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UAircraftEngineComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UAircraftEngineComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	UpdateEnginePower(DeltaTime);
	UpdateOutputThrust();
	UpdateEngineState();
}

void UAircraftEngineComponent::UpdateEnginePower(const float DeltaTime)
{
	if (EngineAccelerationCurve)
	{
		const float NormalizedPower {Power / 100.0f};
		float Acceleration {EngineAccelerationCurve->GetFloatValue(NormalizedPower)};
		Acceleration *= EngineAccelerationScalar;
		Power = FMath::FInterpTo(Power, PowerTarget, DeltaTime, Acceleration);
	}
	else
	{
		Power = PowerTarget;
	}
}

void UAircraftEngineComponent::UpdateEngineState()
{
	const EEngineState InitialState {EngineState};

	if (PowerTarget == 0.0f && Power == 0.0f)
	{
		EngineState = EEngineState::TurnedOff;
	}
	else if ((PowerTarget != 0.0f || Power != 0.0f) && Power < IdlePower)
	{
		EngineState = EEngineState::Starting;
	}
	else if (Power != 0.0f && Power < IdlePower && PowerTarget == 0.0f)
	{
		EngineState = EEngineState::ShuttingDown;
	}
	else
	{
		EngineState = EEngineState::TurnedOn;
	}
	
	if (InitialState != EngineState)
	{
		OnEngineStateChanged.Broadcast(EngineState);
	}
}

void UAircraftEngineComponent::UpdateOutputThrust()
{
	if(PowerToThrustRatioCurve)
	{
		OutputThrust = PowerToThrustRatioCurve->GetFloatValue(Power) * MaxEngineThrust;
		return;
	}

	OutputThrust = 0.0f;
}

