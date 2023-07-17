// Copyright (c) 2023-present Nino Saglia & Tim Verberne. All rights reserved.
// Written by Tim Verberne.

#include "AircraftMovementComponent.h"

DEFINE_LOG_CATEGORY_CLASS(UAircraftMovementComponent, LogAircraftMovement)

UAircraftMovementComponent::UAircraftMovementComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UAircraftMovementComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UAircraftMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

