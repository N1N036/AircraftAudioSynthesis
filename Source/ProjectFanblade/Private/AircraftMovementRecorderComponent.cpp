#include "AircraftMovementRecorderComponent.h"

	UAircraftMovementRecorderComponent::UAircraftMovementRecorderComponent()
	{
		PrimaryComponentTick.bCanEverTick = true;
		SetComponentTickEnabled(true);
		
	}

void UAircraftMovementRecorderComponent::TickComponent(float DeltaTime, ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	if(bRecordMovement)
	{
		//increment the delta time so we can compare with the acumulated delta time in the replay actor. 
		AcumulatedDeltaTime += DeltaTime;
		RecordMovement(GetAircraft()->GetActorTransform(),DeltaTime,GetAircraft()->GetAircraftMovementComponent()->EngineCommands[0].Throttle);
	}
}

void UAircraftMovementRecorderComponent::RecordMovement(const FTransform& CurrentTransform, float DeltaTime, float Throttle)
{
	if(DataAssetInstance)
	{
		FMovementDataStruct NewData;
		NewData.TemporalTransform = CurrentTransform;
		NewData.AcumulatedDeltaTime = DeltaTime;
		NewData.Throttle = Throttle;

		DataAssetInstance->MovementDataArray.Add(NewData);
		
		DataAssetInstance->MarkPackageDirty();
	}
}
