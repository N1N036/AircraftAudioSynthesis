// Copyright (c) 2023-present Nino Saglia & Tim Verberne. All rights reserved.
// Written by Tim Verberne.

#include "Airplane.h"

AAirplane::AAirplane()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AAirplane::BeginPlay()
{
	Super::BeginPlay();
}

void AAirplane::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


