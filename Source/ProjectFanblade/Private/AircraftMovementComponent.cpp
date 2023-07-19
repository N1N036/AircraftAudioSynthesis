// Copyright (c) 2023-present Nino Saglia & Tim Verberne. All rights reserved.
// Written by Tim Verberne.

#include "AircraftMovementComponent.h"
#include "Aircraft.h"
#include "PhysicsEngine/PhysicsAsset.h"

DEFINE_LOG_CATEGORY_CLASS(UAircraftMovementComponent, LogAircraftMovement)

UAircraftMovementComponent::UAircraftMovementComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UAircraftMovementComponent::OnRegister()
{
	Super::OnRegister();

	Aircraft = Cast<AAircraft>(GetOwner());
}

void UAircraftMovementComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UAircraftMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UAircraftMovementComponent::UpdateVelocity(FVector& Vector, const float DeltaTime)
{
	Velocity.Z += Gravity * DeltaTime;
}

float UAircraftMovementComponent::CalculateAircraftDrag() const
{
	if (!Aircraft)
	{
		return 0.0f;
	}
	
	const float DragCoefficient {0.2f};
	const float Drag {static_cast<float>(DragCoefficient * Velocity.SizeSquared())};

	return Drag;
}



