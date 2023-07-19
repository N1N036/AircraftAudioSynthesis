// Copyright (c) 2023-present Nino Saglia & Tim Verberne. All rights reserved.
// Written by Tim Verberne.

#include "AirplaneMovementComponent.h"

#include "AircraftEngineComponent.h"
#include "Airplane.h"

UAirplaneMovementComponent::UAirplaneMovementComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UAirplaneMovementComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UAirplaneMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	UpdateVelocity(Velocity, DeltaTime);

	if (GetAircraft())
	{
		GetAircraft()->AddActorWorldOffset(Velocity * DeltaTime);
	}
}

void UAirplaneMovementComponent::UpdateVelocity(FVector& Vector, const float DeltaTime)
{
	Super::UpdateVelocity(Vector, DeltaTime);
	
	// Calculate forward velocity based on rotation, engine thrust, and drag
	FRotator Rotation {GetOwner()->GetActorRotation()};
	FVector ForwardVector {Rotation.Vector()};
	
	float Thrust = {GetCombinedEngineThrust()};
	float Drag = {CalculateAircraftDrag()};
	
	// Calculate the forward velocity by combining thrust, drag, and the aircraft's current velocity
	FVector ForwardVelocity = ForwardVector * (Thrust - Drag) * DeltaTime;
	
	// Update the velocity vector by adding the forward velocity
	Vector += ForwardVelocity;
}

float UAirplaneMovementComponent::GetCombinedEngineThrust() const
{
	if (!GetAircraft())
	{
		return 0.0f;
	}

	const AAirplane* Airplane {Cast<AAirplane>(GetAircraft())};

	float CombinedThrust {0.0f};
	
	TArray<UAircraftEngineComponent*> Engines {Airplane->GetEngines()};

	for (const UAircraftEngineComponent* Engine : Engines)
	{
		if (Engine)
		{
			CombinedThrust += Engine->GetOutputThrust();
		}
	}

	return CombinedThrust;
}


