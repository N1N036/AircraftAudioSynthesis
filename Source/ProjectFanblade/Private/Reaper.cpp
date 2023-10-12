// Copyright (c) 2023-present Nino Saglia & Tim Verberne. All rights reserved.
// Written by Tim Verberne.
#include "Reaper.h"
#include "ReaperAudioComponent.h"
#include "Components/AudioComponent.h"

AReaper::AReaper()
{
	PrimaryActorTick.bCanEverTick = true;

	ReaperAircraftAudioComponent = CreateDefaultSubobject<UReaperAudioComponent>(TEXT("ReaperAudioComponent"));
	
	PropAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("PropAudioComponent"));
	PropAudioComponent->SetupAttachment(SkeletalMeshComponent);
	PropAudioComponent->AttachToComponent(SkeletalMeshComponent,FAttachmentTransformRules::KeepRelativeTransform);

	MotorAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("MotorAudioComponent"));
	MotorAudioComponent->SetupAttachment(SkeletalMeshComponent);
	MotorAudioComponent->AttachToComponent(SkeletalMeshComponent,FAttachmentTransformRules::KeepRelativeTransform);
}

void AReaper::BeginPlay()
{
	Super::BeginPlay();
}

void AReaper::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//Other variables are sadly not changing in the JSBSim Component. 
	PropSpeed = AircraftMovementComponent->EngineCommands[0].Throttle;
}


