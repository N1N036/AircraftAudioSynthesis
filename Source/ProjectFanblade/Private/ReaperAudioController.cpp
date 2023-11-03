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
}

void UReaperAudioController::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if(!PropAudioComponent){return;}
};