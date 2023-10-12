#pragma once

#include "CoreMinimal.h"
#include "AircraftMovementDataStruct.h"
#include "AircraftMovementDataAsset.generated.h"

UCLASS(Blueprintable)
class UAircraftMovementDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FMovementDataStruct> MovementDataArray;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FTransform OriginalTransform;
	
};