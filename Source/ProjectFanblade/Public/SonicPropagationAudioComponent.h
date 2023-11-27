#pragma once

#include "CoreMinimal.h"
#include "AudioDevice.h"
#include "Components/AudioComponent.h"
#include "SonicPropagationAudioComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FSonicBoomDelegate, FVector, Location, float, MachValue);

UENUM(BlueprintType)
enum class EApplyAudioParamTo : uint8
{
	All
	,Closest
	,Farthest
	
};


USTRUCT(BlueprintType)
struct FSoundHistoryEntry
{
	GENERATED_USTRUCT_BODY()
	
	UPROPERTY(BlueprintReadWrite, Category = "Sound")
	FTransform Transform;

	UPROPERTY(BlueprintReadWrite, Category = "Sound")
	TArray<FAudioParameter> AudioParameters;

	UPROPERTY(BlueprintReadWrite, Category = "Sound")
	float WorldTimeSeconds;

	FVector SpeedOfSourceCmPs;
	
	bool bFlagForRemoval = false;

	TArray<FAudioParameter> GetAudioParameters()
	{
		const TArray<FAudioParameter>& Parameters = AudioParameters;
		return Parameters;
	}
	
	friend bool operator==(const FSoundHistoryEntry& A, const FSoundHistoryEntry& B)
	{
			return A.Transform.GetTranslation().Equals(B.Transform.GetTranslation(), 1e-4) &&
				   A.Transform.GetRotation().Equals(B.Transform.GetRotation(), 1e-4) &&
				   A.Transform.GetScale3D().Equals(B.Transform.GetScale3D(), 1e-4) &&
				   A.WorldTimeSeconds == B.WorldTimeSeconds;
	}
};


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class PROJECTFANBLADE_API USonicPropagationAudioComponent : public USceneComponent
{
	GENERATED_BODY()

public:
	USonicPropagationAudioComponent();
	
	/** Record the audio parameters so that they will be played back at a delayed time taking sonic propagation into account.*/
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Add AudioParameters to history"))
	void AddAudioParametersToCurrentHistory(TArray<FAudioParameter> InValues);

	/** Set the audio parameter instantly, useful for changes in the sound source that aren't caused by the sound source itself.*/
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Set AudioParameters instantly"))
	void ApplyInstantAudioParameters(TArray<FAudioParameter> InValues, EApplyAudioParamTo Case);

	/** We only need the local sound source velocity if we want to calculate the sonic boom intensity.*/
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Set source Velocity value Cm/s"))
	void BlueprintRecordSourceVelocityToCurrentDataEntry(float VelocityIn);

	
	void RecordSourceStateToCurrentDataEntry();
	float GetDopplerPitchMultiplier(FVector InLastListenerLocation, FVector InListenerLocation,
	                                FVector InLastSourceLocation, FVector InSourceLocation, float InDeltaSeconds);

	void SetSpeedOfSound(float InSpeedOfSoundCmPs);

	UPROPERTY(BlueprintAssignable, Category = "Sound")
	FSonicBoomDelegate OnSonicBoom;
	
	UPROPERTY(EditDefaultsOnly)
	USoundBase* SoundBase;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	bool bDrawDebug = false;

protected:
	float SpeedOfSoundCmPs = 34300.0f;
	FVector PrevComponentLocation;

	virtual void BeginPlay() override;
	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	TArray<FSoundHistoryEntry> GetCurrentAudibleSoundData(const FVector& InListenerLocation, float DeltaSeconds);

	TMap<UAudioComponent*,FSoundHistoryEntry> GetAudioComponentDataMap(TArray<FSoundHistoryEntry> AudibleSoundLocations,float CurrentTime);
	

	void CleanUpOldEntries(float CurrentTimeSeconds, float MaxDelaySeconds);

	void ApplyDataToSoundComponent(FSoundHistoryEntry Entry, UAudioComponent* InAudioComponent);
	
	FVector GetListenerLocation()
	{
		GetWorld()->GetAudioDevice()->GetListenerPosition(0, ListenerLocation, false);
		return ListenerLocation;
	}
	FVector LastListenerLocation;
	TArray<FSoundHistoryEntry> SoundHistoryEntries;
	
	FSoundHistoryEntry CurrentAudioDataEntry;
	
	TArray<UAudioComponent*> AudioComponents;
	TMap<UAudioComponent*,float> PendingFadeoutAudioComponents;

	FVector Lerp(FVector A, FVector B, float Alpha)
	{
		return A + Alpha * (B - A);
	}



private:
	float SoundFadeOutTime = .6f;
	float SoundFadeInTime =.6f;
	float TimeUntilCleanup = .6f;
	float MaxDistanceBetweenContinuousSounds = FMath::Pow(10000.0 , 2.0); //100 meter. 
	FVector ListenerLocation;
};




