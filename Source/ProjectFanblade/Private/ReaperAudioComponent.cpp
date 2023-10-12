#include "ReaperAudioComponent.h"


UReaperAudioComponent::UReaperAudioComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	SetComponentTickEnabled(true);

}

void UReaperAudioComponent::BeginPlay()
{
	Super::BeginPlay();
	Reaper = Cast<AReaper>(GetOwner());
	if(!Reaper){return;}
	PropAudioComponent = Reaper->GetPropAudioComponent();
	MotorAudioComponent = Reaper->GetMotorAudioComponent();
}

void UReaperAudioComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if(!PropAudioComponent){return;}

	//for now i get the throttle since i know this returns a value, more setup of the flight model might be needed to get more acurate engine and prop info.
	PropAudioComponent->SetFloatParameter("PropSpeed", Reaper->GetPropSpeed());
	PropAudioComponent->SetFloatParameter("Airspeed",GetAirSpeed());
	PropAudioComponent->SetFloatParameter("DopplerShift",GetDopplerShift());
};