// Copyright (c) 2023-present Nino Saglia & Tim Verberne. All rights reserved.
// Written by Tim Verberne.
#include "Reaper.h"
#include "ReaperAudioController.h"
#include "Components/AudioComponent.h"

AReaper::AReaper()
{
	PrimaryActorTick.bCanEverTick = true;

}

void AReaper::BeginPlay()
{
	Super::BeginPlay();
}

void AReaper::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


