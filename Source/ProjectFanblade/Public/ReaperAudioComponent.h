#pragma once
#include "AircraftAudioComponent.h"
#include "Reaper.h"
#include "Math/Vector.h"
#include "Components/AudioComponent.h"
#include "ReaperAudioComponent.generated.h"

UCLASS(Blueprintable, BlueprintType, ClassGroup = "Aircraft", Meta = (BlueprintSpawnableComponent))
class PROJECTFANBLADE_API UReaperAudioComponent : public UAircraftAudioComponent
{
	GENERATED_BODY()


private:
	UAudioComponent* PropAudioComponent{nullptr};
	UAudioComponent* MotorAudioComponent{nullptr};
	AReaper* Reaper{nullptr};

public:
	UPROPERTY(Interp)
	float PropSpeed{0.0f};

	UPROPERTY(Interp)
	float Airspeed{0.0f};
	

	

	
protected:
	
	
	UReaperAudioComponent();
	
	virtual void BeginPlay() override;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};

