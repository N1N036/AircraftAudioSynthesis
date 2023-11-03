#include "AircraftCommandsRecordReplayComponent.h"

UAircraftCommandsRecordReplayComponent::UAircraftCommandsRecordReplayComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	SetTickGroup(TG_PrePhysics);
}

void UAircraftCommandsRecordReplayComponent::BeginPlay()
{
	UActorComponent::BeginPlay();
}

void UAircraftCommandsRecordReplayComponent::TickComponent(float DeltaTime, ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	UActorComponent::TickComponent(DeltaTime, TickType, ThisTickFunction);

	switch(CurrentMode)
	{
	case ERecordReplayComponentMode::Idle:
		break;
	case ERecordReplayComponentMode::Record:
		UpdateRecordMode();
		break;
	case ERecordReplayComponentMode::Replay:
		UpdateReplayMode();
		break;
	}
}



void UAircraftCommandsRecordReplayComponent::UpdateRecordMode()
{
	UJSBSimMovementComponent* MovementComponent = GetJSBSimMovementComponent();
	if (MovementComponent)
	{
		FAircraftCommandsCollection NewRecord;
		NewRecord.CurrentTimeSeconds = GetWorld()->GetTimeSeconds();
		NewRecord.EngineCommands = MovementComponent->EngineCommands;
		NewRecord.FlightControlCommands = MovementComponent->Commands;
		
		NewRecord.Location = GetOwner()->GetActorLocation();
		NewRecord.Rotation = GetOwner()->GetActorRotation();

        
		CommandsDataAsset->CommandsCollectionEntries.Add(NewRecord);
	}
}


void UAircraftCommandsRecordReplayComponent::UpdateReplayMode()
{
	if (CommandsDataAsset)
	{
		UJSBSimMovementComponent* MovementComponent = GetJSBSimMovementComponent();
		if (MovementComponent)
		{
			int NumberOfEntries = CommandsDataAsset->CommandsCollectionEntries.Num();
			if(NumberOfEntries == 0){return;}
			float CurrentTime = GetWorld()->GetTimeSeconds();

			auto CurrentSession = CommandsDataAsset->CommandsCollectionEntries[CurrentSessionIndex];
			// Check if it's time to apply the next recorded command
			if (CurrentTime >= CurrentSession.CurrentTimeSeconds)
			{
				MovementComponent->Commands = CurrentSession.FlightControlCommands;
				MovementComponent->EngineCommands = CurrentSession.EngineCommands;

				GetOwner()->SetActorLocation(CurrentSession.Location);
				GetOwner()->SetActorRotation(CurrentSession.Rotation);
				
				
				// Move to next session
				CurrentSessionIndex = (CurrentSessionIndex + 1) % NumberOfEntries;
			}
		}
	}
}
