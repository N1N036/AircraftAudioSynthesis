// Copyright (c) 2023-present Nino Saglia & Tim Verberne. All rights reserved.
// Written by Tim Verberne.

#pragma once

#include "CoreMinimal.h"
#include "Aircraft.h"
#include "GameFramework/Pawn.h"
#include "Helicopter.generated.h"

/** Base class for helicopters. */
UCLASS(Abstract, ClassGroup = "Aircraft")
class PROJECTFANBLADE_API AHelicopter : public AAircraft
{
	GENERATED_BODY()

public:
	AHelicopter();

	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;
};
