// Copyright (c) 2023-present Nino Saglia & Tim Verberne. All rights reserved.
// Written by Tim Verberne.

#include "Aircraft.h"

#include "AircraftAudioComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "JSBSimMovementComponent.h"

DEFINE_LOG_CATEGORY_CLASS(AAircraft, LogAircraft)

AAircraft::AAircraft()
{
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;
	
	SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshComponent"));
	SkeletalMeshComponent->AttachToComponent(Root, FAttachmentTransformRules::KeepRelativeTransform, NAME_None);

	AircraftMovementComponent = CreateDefaultSubobject<UJSBSimMovementComponent>(TEXT("JSBSimMovementComponent"));

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(SkeletalMeshComponent);
	CameraBoom->TargetArmLength = 2000; 
	CameraBoom->bUsePawnControlRotation = false; 
	CameraBoom->bEnableCameraLag = true; 
	CameraBoom->CameraLagSpeed = 3.0f;
	CameraBoom->bEnableCameraRotationLag = true;
	CameraBoom->CameraRotationLagSpeed = 3.0f;
	
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); 
	CameraComponent->bUsePawnControlRotation = false;

	PrimaryActorTick.bCanEverTick = true;


}

void AAircraft::BeginPlay()
{
	Super::BeginPlay();
}

void AAircraft::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Get the location of the plane and the camera
	FVector PlaneLocation = SkeletalMeshComponent->GetComponentLocation();
	FVector CameraLocation = CameraComponent->GetComponentLocation();

	// Calculate the direction vector from the camera to the plane
	FVector Direction = PlaneLocation - CameraLocation;
	Direction.Normalize();

	// Calculate the new rotation for the camera to look at the plane
	FRotator DesiredRotation = Direction.Rotation();
    
	// Get the current camera rotation
	FRotator CurrentRotation = CameraComponent->GetComponentRotation();

	// Interpolate between the current rotation and the desired rotation
	FRotator NewRotation = FMath::RInterpTo(CurrentRotation, DesiredRotation, DeltaTime, 2.0f);

	// Apply the new rotation to the camera
	CameraComponent->SetWorldRotation(NewRotation);
}

void AAircraft::SetJSBSIMVelocity(float Vnew)
{
	AircraftMovementComponent->AircraftState.CalibratedAirSpeedKts = Vnew;

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


