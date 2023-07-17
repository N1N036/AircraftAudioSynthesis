// Copyright (c) 2023-present Nino Saglia & Tim Verberne. All rights reserved.
// Written by Tim Verberne.

#pragma once

#include "CoreMinimal.h"
#include "AircraftMovementComponent.h"
#include "Components/ActorComponent.h"
#include "AirplaneMovementComponent.generated.h"

UCLASS(ClassGroup = "Aircraft", Meta = (BlueprintSpawnableComponent))
class PROJECTFANBLADE_API UAirplaneMovementComponent : public UAircraftMovementComponent
{
	GENERATED_BODY()

public:	
	UAirplaneMovementComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	virtual void BeginPlay() override;
};
