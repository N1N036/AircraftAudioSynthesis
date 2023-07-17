// Copyright (c) 2023-present Nino Saglia & Tim Verberne. All rights reserved.
// Written by Tim Verberne.

#include "AircraftAudioComponent.h"

DEFINE_LOG_CATEGORY_CLASS(UAircraftAudioComponent, LogAircraftAudio)

UAircraftAudioComponent::UAircraftAudioComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UAircraftAudioComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UAircraftAudioComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

