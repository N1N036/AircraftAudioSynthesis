#pragma once
#include "AircraftMovementDataAsset.h"
#include "JSBSimMovementComponent.h"
#include "AircraftCommandsRecordReplayComponent.generated.h"
/**
 * This component is responsible for recording and replaying CommandCollection data assets.
 * With this component we can replay the aircraft commands to make interesting scene's and pre recorded maneuvres.
 */

UENUM(BlueprintType)
enum class ERecordReplayComponentMode : uint8
{
	Idle,
	Record,
	Replay
};

USTRUCT(BlueprintType)
struct FAircraftCommandsCollection
{
	GENERATED_BODY()
	float CurrentTimeSeconds;
	TArray<struct FEngineCommand> EngineCommands;
	FFlightControlCommands FlightControlCommands;

	FVector Location;
	FRotator Rotation;
	
};

UCLASS(Blueprintable)
class UAircraftCommandsDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FAircraftCommandsCollection> CommandsCollectionEntries;
	
};

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class PROJECTFANBLADE_API UAircraftCommandsRecordReplayComponent : public UActorComponent
{
	GENERATED_BODY()
	
public:
	/**Choose between idle, recording and replay mode.*/
	UPROPERTY(EditInstanceOnly, DisplayName = "Set the component's mode.")
	ERecordReplayComponentMode CurrentMode;


		
	UPROPERTY(EditInstanceOnly)
	UAircraftCommandsDataAsset* CommandsDataAsset;

		
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
	UAircraftCommandsRecordReplayComponent();
	void BeginPlay() override;
	void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	UPROPERTY()
	UJSBSimMovementComponent* CachedMovementComponent;
	
};
