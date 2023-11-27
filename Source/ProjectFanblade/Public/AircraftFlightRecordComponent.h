#pragma once
#include "CoreMinimal.h"
#include "JSBSimMovementComponent.h"
//#include "DrawDebugHelpers.h"
#include "Components/ActorComponent.h"
#include "AircraftFlightRecordComponent.generated.h"
/**
 * This component is responsible for recording and replaying CommandCollection data assets.
 * With this component we can replay the aircraft commands to make interesting scenes with  recorded maneuvres.
 */


USTRUCT(BlueprintType)
struct FAircraftCommandsCollection
{
	GENERATED_BODY()

	UPROPERTY()
	float CurrentTimeSeconds;
	
	UPROPERTY()
	TArray<struct FEngineCommand> EngineCommands;
	
	UPROPERTY()
	FFlightControlCommands FlightControlCommands;

	UPROPERTY()
	TArray<struct FGear> Gears;

	UPROPERTY()
	FTransform Transform;
};


UENUM(BlueprintType)
enum class ERecordReplayComponentMode : uint8
{
	Idle,
	Record,
	Replay
};


UCLASS(BlueprintType)
class UAircraftCommandsDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	TArray<FAircraftCommandsCollection> CommandsCollectionEntries;
	
};



UCLASS(BlueprintType, meta = (BlueprintSpawnableComponent))
class PROJECTFANBLADE_API UAircraftFlightRecordComponent : public UActorComponent
{
	GENERATED_BODY()
	
public:
	UAircraftFlightRecordComponent();
	

	UFUNCTION(BlueprintGetter)
	ERecordReplayComponentMode GetCurrentMode() const {	return CurrentMode; }



	UPROPERTY(EditAnywhere)
	UAircraftCommandsDataAsset* CommandsDataAsset;

	UPROPERTY(EditInstanceOnly)
	float TimeOffset = 0;

	UFUNCTION(BlueprintCallable,CallInEditor, DisplayName="Draw the recorded flight path")
	void DrawRecordedFlightPath();

	
	UFUNCTION(BlueprintPure, DisplayName = "GetCommandDataLocations")
	TArray<FVector> GetCommandDataLocations()
	{
		TArray<FVector> CommandDataLocations;

		if (CommandsDataAsset)
		{
			for (const auto& Command : CommandsDataAsset->CommandsCollectionEntries)
			{
				CommandDataLocations.Add(Command.Transform.GetLocation());
			}
		}

		return CommandDataLocations;
	}


		
	UJSBSimMovementComponent* GetJSBSimMovementComponent()
	{
		if (!CachedMovementComponent)
		{
			CachedMovementComponent = Cast<UJSBSimMovementComponent>(GetOwner()->GetComponentByClass(UJSBSimMovementComponent::StaticClass()));
		}
		return CachedMovementComponent;
	}
	

protected:
	void UpdateRecordMode();
	void UpdateReplayMode();

	int CurrentSessionIndex;

private:


	UPROPERTY(EditInstanceOnly, BlueprintGetter = GetCurrentMode)
	ERecordReplayComponentMode CurrentMode = ERecordReplayComponentMode::Idle;

	void BeginPlay() override;
	void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	UPROPERTY()
	UJSBSimMovementComponent* CachedMovementComponent;

	float Alpha = -1;

};
