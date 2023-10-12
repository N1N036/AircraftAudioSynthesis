#pragma once
#include "CoreMinimal.h"
#include "AircraftMovementDataStruct.generated.h"


	USTRUCT(BlueprintType)
	struct FMovementDataStruct
	{
		GENERATED_USTRUCT_BODY()

		// Temporal transform
		UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FTransform TemporalTransform;

		// Delta time
		UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float AcumulatedDeltaTime;

		// Throttle value
		UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float Throttle;
};
