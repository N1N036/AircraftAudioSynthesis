#include "ReaperAudioController.h"


UReaperAudioController::UReaperAudioController()
{
	PrimaryComponentTick.bCanEverTick = true;
	SetComponentTickEnabled(true);

}

void UReaperAudioController::BeginPlay()
{
	Super::BeginPlay();
	Reaper = Cast<AReaper>(GetOwner());
	if(!Reaper){return;}
	PropAudioComponent = Reaper->GetPropAudioComponent();
}

void UReaperAudioController::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if(!PropAudioComponent){return;}

	//To expose the parameters to the take recorder for replay use.
	if(Reaper->GetPropSpeed()) PropSpeed = Reaper->GetPropSpeed();
	if(GetAirSpeed()) Airspeed = GetAirSpeed();

	//for now i get the throttle since i know this returns a value, more setup of the flight model might be needed to get more acurate engine and prop info.
	PropAudioComponent->SetFloatParameter("PropSpeed", PropSpeed);
	PropAudioComponent->SetFloatParameter("Airspeed",Airspeed); //AircraftAudioComponent
	PropAudioComponent->SetFloatParameter("DopplerShift",GetDopplerShift()); //AircraftAudioComponent
};