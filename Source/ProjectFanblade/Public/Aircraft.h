// Copyright (c) 2023-present Nino Saglia & Tim Verberne. All rights reserved.
// Written by Tim Verberne.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "JSBSimMovementComponent.h"
#include "Aircraft.generated.h"




class UAircraftAudioComponent;
class UCapsuleComponent;
class UAircraftMovementComponent;

/** Base class for aircraft. */
UCLASS(Abstract, BlueprintType, ClassGroup = "Aircraft")
class PROJECTFANBLADE_API AAircraft : public APawn
{
	GENERATED_BODY()

	DECLARE_LOG_CATEGORY_CLASS(LogAircraft, Log, All)

protected:
	/** The root component of the aircraft. We treat this as the center of gravity. */
	UPROPERTY(EditDefaultsOnly)
	USceneComponent* Root {nullptr};
	
	/** The skeletal mesh component for the aircraft. */
	UPROPERTY(EditDefaultsOnly, BlueprintGetter = GetSkeletalMeshComponent)
	USkeletalMeshComponent* SkeletalMeshComponent {nullptr};

	/** The movement component for the aircraft. */
	UPROPERTY(EditDefaultsOnly, BlueprintGetter = GetAircraftMovementComponent)
	UJSBSimMovementComponent* AircraftMovementComponent {nullptr};

	/** The audio component for the aircraft. */
	UPROPERTY(EditDefaultsOnly, BlueprintGetter = GetAircraftAudioComponent)
	UAircraftAudioController* AircraftAudioController {nullptr};

	/** The SpringArm that will handle the camera's motion and zoom. */
	UPROPERTY(EditDefaultsOnly, BlueprintGetter = GetCameraBoom)
	USpringArmComponent* CameraBoom {nullptr};

	/** The camera of the aircraft. */
	UPROPERTY(EditDefaultsOnly, BlueprintGetter = GetCameraComponent)
	UCameraComponent* CameraComponent {nullptr};

	/** The weight of the aircraft. */
	UPROPERTY(EditDefaultsOnly, BlueprintGetter = GetWeight, Meta = (Units = "kilograms", Delta = "100"))
	int32 Weight {6000};
	
	/** If true, the aircraft is currently in the air and is not touching the ground. */
	UPROPERTY(BlueprintGetter = GetIsAirborne)
	bool IsAirborne {false};

public:
	AAircraft();

	virtual void Tick(float DeltaTime) override;

	/** Exposing the velocity of the JSBSim comp to BP.*/
	UFUNCTION(BlueprintCallable)
	void SetJSBSIMVelocity(float Vnew);

	/** Returns the aircraft's relative height. */
	UFUNCTION(BlueprintCallable)
	double GetAltitude() const;

protected:
	virtual void BeginPlay() override;

public:
	UFUNCTION(BlueprintGetter)
	USkeletalMeshComponent* GetSkeletalMeshComponent() const { return SkeletalMeshComponent; }
	
	UFUNCTION(BlueprintGetter)
	UJSBSimMovementComponent* GetAircraftMovementComponent() const { return AircraftMovementComponent; }

	UFUNCTION(BlueprintGetter)
	UAircraftAudioController* GetAircraftAudioComponent() const { return AircraftAudioController; }

	UFUNCTION(BlueprintGetter)
	USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	
	UFUNCTION(BlueprintGetter)
	UCameraComponent* GetCameraComponent() const { return CameraComponent; }

	UFUNCTION(BlueprintGetter)
	int32 GetWeight() const { return Weight; }
	
	UFUNCTION(BlueprintGetter)
	bool GetIsAirborne() const { return IsAirborne; }
	
};
