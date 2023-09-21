// Copyright (c) 2023-present Nino Saglia & Tim Verberne. All rights reserved.
// Written by Tim Verberne.

#pragma once

#include "CoreMinimal.h"
#include "JSBSimMovementComponent.h"
#include "Components/SceneComponent.h"
#include "Sound/SoundNodeDoppler.h"
#include "AircraftAudioComponent.generated.h"

UCLASS(Blueprintable, BlueprintType, ClassGroup = "Aircraft", Meta = (BlueprintSpawnableComponent))
class PROJECTFANBLADE_API UAircraftAudioComponent : public UActorComponent
{
	GENERATED_BODY()
	
	DECLARE_LOG_CATEGORY_CLASS(LogAircraftAudio, Log, All)

public:
	
protected:
	//Ref to the movement component that contains all flight data.
	UJSBSimMovementComponent* JSBSimMovementComp{nullptr};

	//Data to be used later in specific components to drive audio parameters.
	FAircraftState AircraftState;
	TArray<FEngineState> EngineStates;
	TArray<FEngineCommand> EngineCommands;
	TArray<FGear> Gears;

public:
	UAircraftAudioComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	float CalculateDopplerShift(float speedOfSound, 
								const FVector& observerVelocity, const FVector& observerPosition, 
								const FVector& sourceVelocity, const FVector& sourcePosition);


protected:
	virtual void BeginPlay() override;
};
