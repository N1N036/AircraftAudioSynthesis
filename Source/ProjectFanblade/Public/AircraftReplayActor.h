#include "Aircraft.h"
#include "AircraftMovementDataAsset.h"
#include "AircraftReplayActor.generated.h"

UCLASS(Blueprintable)
class PROJECTFANBLADE_API AAircraftReplayActor : public AActor
{
	GENERATED_BODY()

public:

	
	UPROPERTY(EditInstanceOnly)
	UAircraftMovementDataAsset* MovementDataAsset;

	/**The aircraft that will be moved.*/
	UPROPERTY(EditInstanceOnly)
	TSubclassOf<AAircraft> AircraftBlueprint;


	
private:
	
	FTransform InitialTransform;
	
	float AccumulatedDeltaTime{0.0f};
	int ReplayIndex{0};
	
	AAircraft* Aircraft{nullptr};
	
	FMovementDataStruct CurrentData;
	
	AAircraftReplayActor();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;


	
protected:
	void UpdateAircraftState();

};

