#pragma once
#include "AircraftAudioComponent.h"
#include "Reaper.h"
#include "ReaperAudioComponent.generated.h"

UCLASS(Blueprintable, BlueprintType, ClassGroup = "Aircraft", Meta = (BlueprintSpawnableComponent))
class PROJECTFANBLADE_API UReaperAudioComponent : public UAircraftAudioComponent
{
	GENERATED_BODY()


private:
	UAudioComponent* PropAudioComponent{nullptr};
	UAudioComponent* MotorAudioComponent{nullptr};
	AReaper* Reaper{nullptr};
	
protected:
	
	UReaperAudioComponent();
	
	virtual void BeginPlay() override;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};

