#pragma once
#include "AircraftAudioController.h"
#include "Reaper.h"
#include "Math/Vector.h"
#include "Components/AudioComponent.h"
#include "ReaperAudioController.generated.h"

UCLASS(Blueprintable, BlueprintType, ClassGroup = "Aircraft", Meta = (BlueprintSpawnableComponent))
class PROJECTFANBLADE_API UReaperAudioController : public UAircraftAudioController
{
	GENERATED_BODY()


private:
	UAudioComponent* PropAudioComponent{nullptr};
	AReaper* Reaper{nullptr};

public:
	UPROPERTY(Interp)
	float PropSpeed{0.0f};

	UPROPERTY(Interp)
	float Airspeed{0.0f};
	

	

	
protected:
	
	
	UReaperAudioController();
	
	virtual void BeginPlay() override;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};

