// Copyright (c) 2023-present Nino Saglia & Tim Verberne. All rights reserved.
// Written by Tim Verberne.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "GameFramework/Actor.h"
#include "AircraftAudioController.generated.h"

UCLASS(Blueprintable, BlueprintType, ClassGroup = "Aircraft", Meta = (BlueprintSpawnableComponent))
class PROJECTFANBLADE_API UAircraftAudioController : public UActorComponent
{
	GENERATED_BODY()
	
	DECLARE_LOG_CATEGORY_CLASS(LogAircraftAudio, Log, All)

protected:
	float DopplerShift{0.0};
	float AirSpeed{0.0};

public:
	UAircraftAudioController();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	const float CalculateDopplerShift(float speedOfSound, 
								const FVector& observerVelocity, const FVector& observerPosition, 
								const FVector& sourceVelocity, const FVector& sourcePosition);

	const float GetDopplerShift(){return DopplerShift;}
	
	const float GetAirSpeed(){return AirSpeed;}


protected:
	virtual void BeginPlay() override;
};
