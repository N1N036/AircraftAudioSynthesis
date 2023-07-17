// Copyright (c) 2023-present Nino Saglia & Tim Verberne. All rights reserved.
// Written by Tim Verberne.

#pragma once

#include "CoreMinimal.h"
#include "Airplane.h"
#include "GameFramework/Pawn.h"
#include "Eurofighter.generated.h"

/** The Eurofighter Typhoon is a compact-sized multi-role fighter jet equipped with twin engines.
 *  Its sleek design and powerful engine configuration contribute to its exceptional maneuverability and speed,
 *  enabling it to dominate both air-to-air and air-to-ground combat scenarios. */
UCLASS(Abstract, Blueprintable, ClassGroup = "Aircraft", Meta = (DisplayName = "Eurofighter Typhoon"))
class PROJECTFANBLADE_API AEurofighter : public AAirplane
{
	GENERATED_BODY()

public:
	AEurofighter();

	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;
};
