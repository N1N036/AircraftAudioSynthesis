

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AircraftMovementDataAsset.h"
#include "Aircraft.h"
#include "Engine/World.h"
#include "AircraftMovementRecorderComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class UAircraftMovementRecorderComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAircraftMovementDataAsset* DataAssetInstance;
	
	//Enable to record the movement of the actor in this session.
	UPROPERTY(EditInstanceOnly);
	bool bRecordMovement = false;
	
private:

	float AcumulatedDeltaTime{0.0};

	UAircraftMovementRecorderComponent();
	void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	void RecordMovement(const FTransform& CurrentTransform, float DeltaTime, float Throttle);
	

	

	AAircraft* UAircraftMovementRecorderComponent::GetAircraft() const
	{
		AActor* Owner = GetOwner();
		if(!Owner)
		{
			UE_LOG(LogTemp, Warning, TEXT("Movement Recorder Component is not attached to any actor."));
			return nullptr;
		}

		AAircraft* AircraftOwner = Cast<AAircraft>(Owner);
		if(!AircraftOwner)
		{
			UE_LOG(LogTemp, Warning, TEXT("Movement Recorder Component is not attached to an AAircraft."));
			return nullptr;
		}

		return AircraftOwner;
	}
};


