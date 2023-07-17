// Copyright (c) 2023-present Nino Saglia & Tim Verberne. All rights reserved.
// Written by Tim Verberne.

#pragma once

#include "CoreMinimal.h"
#include "Airplane.h"
#include "GameFramework/Pawn.h"
#include "Hercules.generated.h"

/** The C130J Super Hercules is a large-sized turboprop aircraft.
 *  With four powerful engines, it excels in transporting heavy loads. */
UCLASS(Abstract, Blueprintable, ClassGroup = "Aircraft", Meta = (DisplayName = "C-130J Super Hercules"))
class PROJECTFANBLADE_API AHercules : public AAirplane
{
	GENERATED_BODY()

public:
	AHercules();

	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;
};
