// Copyright (c) 2023-present Nino Saglia & Tim Verberne. All rights reserved.
// Written by Tim Verberne.

#pragma once

#include "CoreMinimal.h"
#include "Aircraft.h"
#include "GameFramework/Pawn.h"
#include "Airplane.generated.h"

class UAircraftEngineComponent;
/** Base class for airplanes. */
UCLASS(Abstract, ClassGroup = "Aircraft")
class PROJECTFANBLADE_API AAirplane : public AAircraft
{
	GENERATED_BODY()

private:
	UPROPERTY(BlueprintGetter = GetEngines)
	TArray<UAircraftEngineComponent*> Engines;

public:
	AAirplane();

	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "Input")
	void AddThrottleInput(const float Value);

protected:
	virtual void BeginPlay() override;

public:
	UFUNCTION(BlueprintGetter)
	FORCEINLINE TArray<UAircraftEngineComponent*> GetEngines() const { return Engines; }
};
