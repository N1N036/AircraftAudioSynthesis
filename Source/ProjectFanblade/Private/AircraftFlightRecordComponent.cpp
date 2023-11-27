#include "AircraftFlightRecordComponent.h"


UAircraftFlightRecordComponent::UAircraftFlightRecordComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	SetTickGroup(TG_PrePhysics);
}




void UAircraftFlightRecordComponent::BeginPlay()
{
	Super::BeginPlay();

	if(CurrentMode == ERecordReplayComponentMode::Replay)
	{
		auto MovementComp = GetOwner()->GetComponentByClass(UJSBSimMovementComponent::StaticClass());
		if(MovementComp)
		{
			MovementComp->SetComponentTickEnabled(false);
		}
	}

	if(CurrentMode == ERecordReplayComponentMode::Record)
	{
		if(!CommandsDataAsset){return;}
		
		CommandsDataAsset->CommandsCollectionEntries.Empty();
	}
}

void UAircraftFlightRecordComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	
	//After recording, set to replay for ease of use. 
	if(CurrentMode == ERecordReplayComponentMode::Record)
	{
		CurrentMode = ERecordReplayComponentMode::Replay;
	}
}

void UAircraftFlightRecordComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                                           FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

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
	default:
		break;
	}
}




void UAircraftFlightRecordComponent::DrawRecordedFlightPath()
{
	if (CommandsDataAsset)
	{
		for (const auto& Command : CommandsDataAsset->CommandsCollectionEntries)
		{
			FVector Location = Command.Transform.GetLocation();

			DrawDebugPoint(
				GetWorld(), 
				Location, 
				10.0f,
				FColor::Green, 
				false,
				600.0f
			);
		}
	}
}




void UAircraftFlightRecordComponent::UpdateRecordMode()
{
	UJSBSimMovementComponent* MovementComponent = GetJSBSimMovementComponent();
	if (MovementComponent)
	{
		FAircraftCommandsCollection NewRecord;
		NewRecord.CurrentTimeSeconds = GetWorld()->GetTimeSeconds();
		NewRecord.EngineCommands = MovementComponent->EngineCommands;
		NewRecord.FlightControlCommands = MovementComponent->Commands;
		NewRecord.Gears = MovementComponent->Gears;

		NewRecord.Transform = GetOwner()->GetActorTransform();
        if(!CommandsDataAsset){return;}
		CommandsDataAsset->CommandsCollectionEntries.Add(NewRecord);
		CommandsDataAsset->MarkPackageDirty();
	}
}


void UAircraftFlightRecordComponent::UpdateReplayMode()
{
    if (CommandsDataAsset)
    {
        UJSBSimMovementComponent* MovementComponent = GetJSBSimMovementComponent();
        if (MovementComponent)
        {
            int NumberOfEntries = CommandsDataAsset->CommandsCollectionEntries.Num();
            if(NumberOfEntries == 0){return;}
            float CurrentTime = GetWorld()->GetTimeSeconds() + TimeOffset;
            float LatestTimeInEntries = CommandsDataAsset->CommandsCollectionEntries[NumberOfEntries - 1].CurrentTimeSeconds;

            //Wrap the time around depending on the last history entry.
            CurrentTime = FMath::Fmod(CurrentTime, LatestTimeInEntries);

            FAircraftCommandsCollection CurrentSession;
            FAircraftCommandsCollection NextSession;
            Alpha = -1;
            int TimeOutCounter = 100000;
            while(Alpha < 0 || Alpha > 1 && CurrentTime < LatestTimeInEntries)
            {
                TimeOutCounter--;
                if(TimeOutCounter < 0)
                {
                    break;
                }
                CurrentSession = CommandsDataAsset->CommandsCollectionEntries[CurrentSessionIndex];
                NextSession = CommandsDataAsset->CommandsCollectionEntries[(CurrentSessionIndex + 1) % NumberOfEntries];
                
                float TimeSinceLastSession =  CurrentTime - CurrentSession.CurrentTimeSeconds;
                float TimeInterval = NextSession.CurrentTimeSeconds - CurrentSession.CurrentTimeSeconds;
                Alpha = TimeSinceLastSession / TimeInterval;

                if(Alpha > 1)
                {
                    CurrentSessionIndex = (CurrentSessionIndex + 1) % NumberOfEntries;
                }
                else if(Alpha < 0)
                {
                    CurrentSessionIndex = FMath::Abs((CurrentSessionIndex + NumberOfEntries - 1) % NumberOfEntries);
                }
                else
                {
                    break; // Alpha is between 0 and 1
                }

                if(CurrentTime > LatestTimeInEntries)
                {   

                    break;
                }           
            }

            FTransform BlendedTransform;
            BlendedTransform.Blend(CurrentSession.Transform, NextSession.Transform, Alpha);

            GetOwner()->SetActorTransform(BlendedTransform);
            MovementComponent->Commands = CurrentSession.FlightControlCommands;
            MovementComponent->EngineCommands = CurrentSession.EngineCommands;
			MovementComponent->Gears = CurrentSession.Gears;

        }
    }
}


