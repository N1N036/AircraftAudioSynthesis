#include "SonicPropagationAudioComponent.h"
#include "AudioDevice.h"
#include "DrawDebugHelpers.h" 
#include "FrameTypes.h"
/**
 * @class USonicPropagationAudioComponent
 * @brief Replays a sound taking sonic propagation into consideration.
 * The audio components will be delayed and played at locations where a listener would experience them taking sound propagation into consideration.
 * If this source would move faster than the speed of sound, the audio can be heard at multiple locations at the same time.
 *
 * This component should be attached to any actor that is intended to emit sound that respects sound propagation physics.
 */
USonicPropagationAudioComponent::USonicPropagationAudioComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
    SetComponentTickInterval(0.0);
    SetComponentTickEnabled(true);
}

void USonicPropagationAudioComponent::BeginPlay()
{
    Super::BeginPlay();
}

void USonicPropagationAudioComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
    
    UWorld* World = GetWorld();
    if (!World){return;}
    
    //Clean up any data that was added too long ago.

    CleanUpOldEntries(World->GetTimeSeconds(),60);
    
    //Pass the current state of the sound component to the history for later use. 
    SoundHistoryEntries.Add(CurrentAudioDataEntry);
    CurrentAudioDataEntry.AudioParameters.Empty();
    RecordSourceStateToCurrentDataEntry();
    
    //Find the state (location and sound parameters) of each audio component that you would hear from a source considering the speed of sound.
    TArray<FSoundHistoryEntry> AudibleSoundStates = GetCurrentAudibleSoundData(GetListenerLocation(),DeltaTime);

    TMap<UAudioComponent*,FSoundHistoryEntry> SourceTargetMap = GetAudioComponentDataMap(AudibleSoundStates,World->GetTimeSeconds());

    for (auto& Elem : SourceTargetMap)
    {
        UAudioComponent* SoundComp = Elem.Key;
        FSoundHistoryEntry SoundDataEntry = Elem.Value;

        if(SoundComp)
        {
            ApplyDataToSoundComponent(SoundDataEntry, SoundComp);
        }
    }
}

void USonicPropagationAudioComponent::AddAudioParametersToCurrentHistory(TArray<FAudioParameter> InValues)
{
    for(FAudioParameter InValue : InValues)
    {
        CurrentAudioDataEntry.AudioParameters.Add(InValue);
    }
}

void USonicPropagationAudioComponent::ApplyInstantAudioParameters(TArray<FAudioParameter> InValues, EApplyAudioParamTo Case )
{
    TArray<UAudioComponent*> ComponentsToApplyParameters;
    switch (Case)
    {
    case EApplyAudioParamTo::Closest:
        {
            __int64 LargestDistance = INT_MAX;
            for(UAudioComponent* AudioComp :AudioComponents)
            {
                
                int DistSquaredBetweenSoundCompAndTarget = FVector::DistSquared(AudioComp->GetComponentLocation(),GetListenerLocation());
                if(DistSquaredBetweenSoundCompAndTarget < LargestDistance) 
                {
                    LargestDistance = DistSquaredBetweenSoundCompAndTarget;
                    ComponentsToApplyParameters.Add(AudioComp);
                }
            }
            break;
        }
    case EApplyAudioParamTo::Farthest:
        {
            __int64 SmallestDistance = 0;
            for(UAudioComponent* AudioComp :AudioComponents)
            {
                
                int DistSquaredBetweenSoundCompAndTarget = FVector::DistSquared(AudioComp->GetComponentLocation(),GetListenerLocation());
                if(DistSquaredBetweenSoundCompAndTarget > SmallestDistance) 
                {
                    SmallestDistance = DistSquaredBetweenSoundCompAndTarget;
                    ComponentsToApplyParameters.Add(AudioComp);
                }
            }
            break;
        }
    case EApplyAudioParamTo::All:
        {
            ComponentsToApplyParameters = AudioComponents;
            break;
        }
    }

    for(UAudioComponent* CompToApply: ComponentsToApplyParameters)
    {
        CompToApply->SetParameters(MoveTemp(InValues));
    }
    
}

void USonicPropagationAudioComponent::BlueprintRecordSourceVelocityToCurrentDataEntry(float VelocityIn)
{
    CurrentAudioDataEntry.SpeedOfSourceCmPs = VelocityIn;
}

void USonicPropagationAudioComponent::RecordSourceStateToCurrentDataEntry()
{
    CurrentAudioDataEntry.Transform = GetComponentTransform();
    CurrentAudioDataEntry.WorldTimeSeconds = GetWorld()->GetTimeSeconds();
    CurrentAudioDataEntry.AudioParameters.AddZeroed();
    PrevCompLocation = GetComponentLocation();
}



void USonicPropagationAudioComponent::SetSpeedOfSound(float InSpeedOfSoundCmPs)
{
    SpeedOfSoundCmPs = InSpeedOfSoundCmPs;
}



void USonicPropagationAudioComponent::ApplyDataToSoundComponent(FSoundHistoryEntry Entry, UAudioComponent* InAudioComponent) 
{
    if(!InAudioComponent){return;}
    InAudioComponent->SetWorldTransform(Entry.Transform);
        InAudioComponent->SetParameters(MoveTemp(Entry.AudioParameters));
        if (bDrawDebug)
        {
            DrawDebugSphere(GetWorld(), Entry.Transform.GetLocation(), 30.f, 4, FColor::Purple, false, 20.f, 0, 20.f);
        }
}



TArray<FSoundHistoryEntry> USonicPropagationAudioComponent::GetCurrentAudibleSoundData(const FVector& InListenerLocation,float DeltaSeconds)
{
    float CurrentTime = GetWorld()->GetTimeSeconds();
    FSoundHistoryEntry* PreEntry = nullptr;
    bool bSoundIsPostAudible = false;
    bool bPreviousConditionState = false;
    TArray<FSoundHistoryEntry> PreInterpolatedSoundLocations;
    TArray<FSoundHistoryEntry> InterpolatedSoundLocations;
    
    //Sort from high to low world time so we check never entries first.
    SoundHistoryEntries.Sort([](const FSoundHistoryEntry& A, const FSoundHistoryEntry& B) {
    return A.WorldTimeSeconds > B.WorldTimeSeconds;
    });
    for (FSoundHistoryEntry& Entry : SoundHistoryEntries)
    {
        
        if (bDrawDebug)
        {
            FVector DebugLocation = Entry.Transform.GetLocation();
            DrawDebugSphere(GetWorld(), DebugLocation, 50.f, 2, FColor::Yellow, false, 0.f, 0, 10.f);
        }
        float CurrentElapsedTime = CurrentTime - Entry.WorldTimeSeconds;
        float CurrentTimeRequired = FVector::Distance(Entry.Transform.GetLocation(), InListenerLocation) / SpeedOfSoundCmPs;

        bSoundIsPostAudible = CurrentElapsedTime >= CurrentTimeRequired;
        
        if(CurrentElapsedTime < CurrentTimeRequired)
        {
            PreEntry = &Entry;
        }

        
        if (bSoundIsPostAudible && bSoundIsPostAudible != bPreviousConditionState)
        {
            if(!PreEntry)
            {
                InterpolatedSoundLocations.Add(Entry);
            }
            else
            {
                if(bDrawDebug)
                {
                    FVector Start = Entry.Transform.GetLocation();
                    FVector End = PreEntry->Transform.GetLocation();
                    DrawDebugDirectionalArrow(GetWorld(), Start, End, 400.f, FColor::Green, false, 50, 0, 10);
                }
                
                FTransform BlendedTransform;

                //Perform a binary approximation to find the right alpha value for ElapsedTime = TimeRequired.
                float AlphaLow = 0.0f, AlphaHigh = 1.0f, Alpha = 0.5f;
                const float Tolerance = 0.001f;
                while (AlphaHigh - AlphaLow > Tolerance) 
                {
                    Alpha = (AlphaLow + AlphaHigh) / 2.0f;

                    float ElapsedTime = CurrentTime - FMath::Lerp(Entry.WorldTimeSeconds,PreEntry->WorldTimeSeconds,Alpha);
                    float TimeRequired = FVector::Distance(Lerp(Entry.Transform.GetLocation(),PreEntry->Transform.GetLocation(), Alpha), InListenerLocation) / SpeedOfSoundCmPs;

                    if ( ElapsedTime > TimeRequired)
                    {
                        AlphaLow = Alpha;
                    } 
                    else 
                    {
                        AlphaHigh = Alpha;
                    }
                    
                }
                if(Alpha < 0)
                {
                    InterpolatedSoundLocations.Add(Entry);
                }
                else if(Alpha > 1)
                {
                    InterpolatedSoundLocations.Add(*PreEntry);
                }
                else
                {
                    BlendedTransform.Blend( Entry.Transform,PreEntry->Transform,  Alpha);

                    FSoundHistoryEntry SoundDataOut;
                    SoundDataOut.Transform = BlendedTransform;
                    SoundDataOut.AudioParameters = Entry.AudioParameters;
                    SoundDataOut.SpeedOfSourceCmPs = FMath::Lerp(Entry.SpeedOfSourceCmPs,PreEntry->SpeedOfSourceCmPs, Alpha);
                    InterpolatedSoundLocations.Add(SoundDataOut);
                }
            }
        }
        //If it isn't the first sound in the list of entries and it is past the audible threshold, we can remove it.
        if(bSoundIsPostAudible && bPreviousConditionState == bSoundIsPostAudible)
        {
            Entry.bFlagForRemoval = true;
        }
        
        bPreviousConditionState = bSoundIsPostAudible;

    }
    return InterpolatedSoundLocations;
}
    


TMap<UAudioComponent*,FSoundHistoryEntry> USonicPropagationAudioComponent::GetAudioComponentDataMap(TArray<FSoundHistoryEntry> AudibleSoundLocations,float CurrentTime)
{
    
    TMap<UAudioComponent*,FSoundHistoryEntry> ComponentAndDataMap;
    int MaxSimultaneousSounds = 4;


    
    for(FSoundHistoryEntry AudibleSoundLocation : AudibleSoundLocations)
    {
        UAudioComponent* AppropriateAudioComponent = nullptr;

    //Get the closest sound component to the target location.
    float SmallestDistance = 100000000;
    if (!AppropriateAudioComponent)
    {
        
        for (UAudioComponent* AudioComponent : AudioComponents)
        {
            if(!AudioComponent){continue;}

            float DistSquaredBetweenSoundCompAndTarget = FVector::DistSquared(AudioComponent->GetComponentLocation(),AudibleSoundLocation.Transform.GetLocation());
            if(DistSquaredBetweenSoundCompAndTarget < SmallestDistance) 
            {
                SmallestDistance = DistSquaredBetweenSoundCompAndTarget;
                AppropriateAudioComponent = AudioComponent;
            }
        }
    }
        /**
         * A sonic boom pressure wave has reached the listener if:
         *  - A sound had become audioble at a new location.
         *  - the source velocity is above the speed of sound at the time this sound was created.
         */
        if(!AppropriateAudioComponent && AudibleSoundLocation.SpeedOfSourceCmPs > SpeedOfSoundCmPs && SoundHistoryEntries.Num() > 1)
        {
            FVector Location = AudibleSoundLocation.Transform.GetLocation();
            float MachValue = AudibleSoundLocation.SpeedOfSourceCmPs / SpeedOfSoundCmPs;
            OnSonicBoom.Broadcast(Location, MachValue);
        }

    //Check if we can make a audio component.
    if (!AppropriateAudioComponent && AudioComponents.Num() < MaxSimultaneousSounds)
    {
        AppropriateAudioComponent = NewObject<UAudioComponent>(this, UAudioComponent::StaticClass());
        AppropriateAudioComponent->SetSound(SoundBase);
        ApplyDataToSoundComponent(AudibleSoundLocation,AppropriateAudioComponent);
        AppropriateAudioComponent->bAutoDestroy = false;
        AppropriateAudioComponent->SetMobility(EComponentMobility::Movable);
        AppropriateAudioComponent->RegisterComponent();
        AppropriateAudioComponent->Play();
        AudioComponents.Add(AppropriateAudioComponent);

        // Draw debug sphere at the new audio component's location
        if (bDrawDebug)
        {
            FVector DebugLocation = AppropriateAudioComponent->GetComponentLocation();
            DrawDebugSphere(GetWorld(), DebugLocation, 400.f, 12, FColor::Red, false, 10.f, 0, 10.f);
        }
    }
    
    //Get the furthest sound component from the listener.
    if (!AppropriateAudioComponent)
    {
        float LargestDistance = 0;
        for (UAudioComponent* AudioComponent : AudioComponents)
        {
            if(!AudioComponent){continue;}
            float DistSquaredBetweenSoundCompAndTarget = FVector::DistSquared(AudioComponent->GetComponentLocation(),ListenerLocation);
            if(DistSquaredBetweenSoundCompAndTarget > LargestDistance) 
            {
                LargestDistance = DistSquaredBetweenSoundCompAndTarget;
                AppropriateAudioComponent = AudioComponent;
            }
        }
    }
    

        ComponentAndDataMap.Add(AppropriateAudioComponent,AudibleSoundLocation);
    }
    
    float PendingToFadeOutTime = 1.0f;
    for(UAudioComponent* Comp : AudioComponents)
    {
        if(!ComponentAndDataMap.Contains(Comp))
        {
            if(!PendingFadeoutAudioComponents.Contains(Comp))
            {
                PendingFadeoutAudioComponents.Add(Comp,GetWorld()->GetTimeSeconds());
            }
            
            //find the comp and the time it was added to the queue, check if it should be faded out.
            if(*PendingFadeoutAudioComponents.Find(Comp) < GetWorld()->GetTimeSeconds() - PendingToFadeOutTime)
            {
                Comp->FadeOut(.5,0);
                PendingFadeoutAudioComponents.Remove(Comp);
            }
        }
        else
        {
            PendingFadeoutAudioComponents.Remove(Comp);
        }
    }
    return ComponentAndDataMap;
}

void USonicPropagationAudioComponent::CleanUpOldEntries(float CurrentTimeSeconds, float MaxDelaySeconds)
{

    for (auto It = SoundHistoryEntries.CreateIterator(); It; ++It)
    {
        FSoundHistoryEntry& Entry = *It;
        if(Entry.bFlagForRemoval)
        {
        It.RemoveCurrent();
        }
        else if (Entry.WorldTimeSeconds < CurrentTimeSeconds - MaxDelaySeconds)
        {
            It.RemoveCurrent();
        }
    }
}



