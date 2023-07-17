// Copyright (c) 2023-present Nino Saglia & Tim Verberne. All rights reserved.
// Written by Tim Verberne.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "AircraftEngineComponent.generated.h"

UENUM(BlueprintType)
enum class EEngineState : uint8
{
	TurnedOff				UMETA(DisplayName = "Turned Off"),
	TurnedOn				UMETA(DisplayName = "Turned On"),
	Starting				UMETA(DisplayName = "Starting"),
	ShuttingDown			UMETA(DisplayName = "Shutting Down"),
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEngineStateChangedDelegate, EEngineState, NewState);

UCLASS(Abstract, ClassGroup = "Aircraft", Meta = (BlueprintSpawnableComponent))
class PROJECTFANBLADE_API UAircraftEngineComponent : public USceneComponent
{
	GENERATED_BODY()

	DECLARE_LOG_CATEGORY_CLASS(LogAircraftEngine, Log, All)

public:
	UPROPERTY(BlueprintAssignable, Category = "Delegates")
	FOnEngineStateChangedDelegate OnEngineStateChanged;

protected:
	/** The current power level of the engine. */
	UPROPERTY(BlueprintGetter = GetPower)
	float Power {0.0f};

	/** The target power level of the engine. */
	UPROPERTY(BlueprintGetter = GetPowerTarget, BlueprintSetter = SetPower)
	float PowerTarget {0.0f};

	/** The idle power level. This is the minimum power level the engine is allowed to run at. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Engine|Power", Meta = (DisplayName = "Idle Power Level"))
	float IdlePower {50.0f};

	/** The maximum engine thrust. This defines the maximum force that the engine can exert. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Thrust", Meta = (DisplayName = "Max Engine Thrust",
		ForceUnits = "lbf", Delta = "10", ClampMin = "0", UIMin = "10", UIMax = "100000"))
	float MaxEngineThrust;

	/** The power to thrust ratio curve. This defines the relationship between the engine's power level and its thrust. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Thrust", Meta = (DisplayName = "Power To Thrust Ratio"))
	UCurveFloat* PowerToThrustRatioCurve;

	/** Global scalar for the engine's acceleration speed. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Engine", Meta = (DisplayName = "Engine Acceleration Scalar",
		ClampMin = "0.1", ClampMax = "2", UIMin = "0.1", UIMax = "2"))
	float EngineAccelerationScalar;

	/** The engine acceleration curve. This defines how quickly the engine can accelerate. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Engine", Meta = (DisplayName = "Engine Acceleration"))
	UCurveFloat* EngineAccelerationCurve;
	
	/** The current output thrust of the engine. */
	UPROPERTY(BlueprintGetter = GetOutputThrust)
	float OutputThrust {0.0f};

private:
	/** The current engine state. */
	UPROPERTY(BlueprintGetter = GetEngineState)
	EEngineState EngineState {EEngineState::TurnedOff};
	

public:	
	UAircraftEngineComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	virtual void BeginPlay() override;

private:
	void UpdateEnginePower(const float DeltaTime);

	void UpdateOutputThrust();

	void UpdateEngineState();

public:
	UFUNCTION(BlueprintGetter)
	FORCEINLINE float GetPower() const { return Power; }

	UFUNCTION(BlueprintGetter)
	FORCEINLINE float GetPowerTarget() const { return PowerTarget; }

	UFUNCTION(BlueprintSetter)
	FORCEINLINE void SetPower(const float Value) { PowerTarget = FMath::Clamp(Value, 0.0f, 100.0f); }

	UFUNCTION(BlueprintGetter)
	FORCEINLINE float GetOutputThrust() const { return OutputThrust; }

	UFUNCTION(BlueprintGetter)
	FORCEINLINE EEngineState GetEngineState() const { return EngineState; }
};
