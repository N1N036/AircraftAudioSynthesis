#include "AircraftReplayActor.h"

AAircraftReplayActor::AAircraftReplayActor()
{

}

void AAircraftReplayActor::BeginPlay()
{
	InitialTransform = GetActorTransform();

	Aircraft = GetWorld()->SpawnActor<AAircraft>(AircraftBlueprint, InitialTransform);
}

void AAircraftReplayActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	

	//iterate through the replay and reset to 0 when out of bounds of the movement data. 
	if(MovementDataAsset && ReplayIndex < MovementDataAsset->MovementDataArray.Num())
	{
		CurrentData = MovementDataAsset->MovementDataArray[ReplayIndex];
		
		AccumulatedDeltaTime += DeltaTime;

		if(AccumulatedDeltaTime >= CurrentData.AcumulatedDeltaTime)
		{
			ReplayIndex ++;
		}
	}
	else
	{
		ReplayIndex = 0;
	}
	
	UpdateAircraftState();
}

void AAircraftReplayActor::UpdateAircraftState()
{
	FTransform AircraftTransform = InitialTransform + CurrentData.TemporalTransform;
	float throttle = CurrentData.Throttle;

	Aircraft->SetActorTransform(AircraftTransform);
}