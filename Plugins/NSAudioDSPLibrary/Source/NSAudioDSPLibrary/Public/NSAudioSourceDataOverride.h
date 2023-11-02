// Copyright (c) 2022 Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#pragma once

#include "IAudioExtensionPlugin.h"
#include "AudioDevice.h"


DECLARE_LOG_CATEGORY_EXTERN(LogAcousticsNative, Log, All);

class FNSAudioSourceDataOverride : public IAudioModulationManager
{
public:
    virtual void GetSourceDataOverrides(
        const uint32 SourceId, const FTransform& InListenerTransform, FWaveInstance* InOutWaveInstance);

private:
    inline FName GetSourceName(const uint32 SourceId)
    {
        return FName(FString::Printf(TEXT("Source_%d"), SourceId));
    }

private:

};