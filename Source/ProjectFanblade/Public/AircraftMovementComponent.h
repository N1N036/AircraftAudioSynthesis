// Copyright (c) 2023-present Nino Saglia & Tim Verberne. All rights reserved.
// Written by Tim Verberne.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "AircraftMovementComponent.generated.h"

class AAircraft;

UCLASS(BlueprintType, ClassGroup = "Aircraft", Meta = (BlueprintSpawnableComponent))
class PROJECTFANBLADE_API UAircraftMovementComponent : public UActorComponent
{
	GENERATED_BODY()

	DECLARE_LOG_CATEGORY_CLASS(LogAircraftMovement, Log, All)

protected:
	UPROPERTY(BlueprintGetter = GetVelocity)
	FVector Velocity {FVector::ZeroVector};

	static constexpr float Gravity {-9.81f};

private:
	UPROPERTY(BlueprintGetter = GetAircraft)
	AAircraft* Aircraft {nullptr};

public:	
	UAircraftMovementComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	virtual void OnRegister() override;
	virtual void BeginPlay() override;

	virtual void UpdateVelocity(FVector& Vector, const float DeltaTime);

	float CalculateAircraftDrag() const;
	
public:
	UFUNCTION(BlueprintGetter)
	AAircraft* GetAircraft() const { return Aircraft; }
	
	UFUNCTION(BlueprintGetter)
	FVector GetVelocity() const { return Velocity; }
};
