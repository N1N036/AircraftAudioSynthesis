// Copyright (c) 2023-present Nino Saglia & Tim Verberne. All rights reserved.
// Written by Tim Verberne.

#pragma once

#include "CoreMinimal.h"
#include "Aircraft.h"
#include "GameFramework/Pawn.h"
#include "Airplane.generated.h"

/** Base class for airplanes. */
UCLASS(Abstract, ClassGroup = "Aircraft")
class PROJECTFANBLADE_API AAirplane : public AAircraft
{
	GENERATED_BODY()

public:
	AAirplane();

	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;
};
