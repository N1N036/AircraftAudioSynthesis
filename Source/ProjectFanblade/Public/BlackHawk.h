// Copyright (c) 2023-present Nino Saglia & Tim Verberne. All rights reserved.
// Written by Tim Verberne.

#pragma once

#include "CoreMinimal.h"
#include "Helicopter.h"
#include "GameFramework/Pawn.h"
#include "BlackHawk.generated.h"

/** The UH-60 Black Hawk is a medium-sized utility helicopter powered by twin engines.
 *  It features a robust construction and is known for its versatility in various missions,
 *  thanks to its moderate size and engine configuration. */
UCLASS(Abstract, Blueprintable, ClassGroup = "Aircraft", Meta = (DisplayName = "UH-60 Black Hawk"))
class PROJECTFANBLADE_API ABlackHawk : public AHelicopter
{
	GENERATED_BODY()

public:
	ABlackHawk();

	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;
};
