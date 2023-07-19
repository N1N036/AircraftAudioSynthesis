// Copyright (c) 2023-present Nino Saglia & Tim Verberne. All rights reserved.
// Written by Tim Verberne.

#include "Airplane.h"
#include "AircraftEngineComponent.h"

AAirplane::AAirplane()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AAirplane::BeginPlay()
{
	Super::BeginPlay();

	TInlineComponentArray<UActorComponent*> Components;
	GetComponents(Components);
	for (UActorComponent* Component : Components)
	{
		if (UAircraftEngineComponent* EngineComponent = Cast<UAircraftEngineComponent>(Component))
		{
			Engines.Add(EngineComponent);
		}
	}
}

void AAirplane::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AAirplane::AddThrottleInput(const float Value)
{
	for (UAircraftEngineComponent* Engine : Engines)
	{
		if (Engine) 
		{
			const float CurrentPower {Engine->GetPowerTarget()};
			const float NewPower {CurrentPower + Value};
			Engine->SetPower(NewPower);
		}
	}
}


