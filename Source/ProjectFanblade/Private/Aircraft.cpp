// Copyright (c) 2023-present Nino Saglia & Tim Verberne. All rights reserved.
// Written by Tim Verberne.

#include "Aircraft.h"
#include "AircraftMovementComponent.h"

DEFINE_LOG_CATEGORY_CLASS(AAircraft, LogAircraft)

AAircraft::AAircraft()
{
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;
	
	SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshComponent"));
	SkeletalMeshComponent->AttachToComponent(Root, FAttachmentTransformRules::KeepRelativeTransform, NAME_None);

	AircraftMovementComponent = CreateDefaultSubobject<UAircraftMovementComponent>(TEXT("MovementComponent"));
}

void AAircraft::BeginPlay()
{
	Super::BeginPlay();
}

void AAircraft::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

double AAircraft::GetAltitude() const
{
	const FVector TraceStart {GetActorLocation()};
	const FVector TraceEnd {TraceStart - FVector(0, 0, 10000)};

	FHitResult Hit;
	
	if (bool IsHit {GetWorld()->LineTraceSingleByChannel(Hit, TraceStart, TraceEnd, ECC_Visibility)})
	{
		return (Hit.ImpactPoint - TraceStart).Size();
	}
	
	return -1;
}


