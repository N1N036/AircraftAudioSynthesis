// Copyright (c) 2023-present Nino Saglia & Tim Verberne. All rights reserved.
// Written by Tim Verberne.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "AircraftAudioComponent.generated.h"

UCLASS(Abstract, ClassGroup = "Aircraft", Meta = (BlueprintSpawnableComponent))
class PROJECTFANBLADE_API UAircraftAudioComponent : public USceneComponent
{
	GENERATED_BODY()
	
	DECLARE_LOG_CATEGORY_CLASS(LogAircraftAudio, Log, All)

public:	
	UAircraftAudioComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	virtual void BeginPlay() override;
};
