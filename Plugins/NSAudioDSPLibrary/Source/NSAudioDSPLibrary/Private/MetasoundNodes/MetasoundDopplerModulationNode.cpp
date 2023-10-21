#include "MetasoundNodes/MetasoundDopplerModulationNode.h"
#include "MetasoundOperatorSettings.h"



#define LOCTEXT_NAMESPACE "MetasoundStandardNodes_DopplerModulationNode"

namespace Metasound
{
    //------------------------------------------------------------------------------------
    // FDopplerModulationOperator
    //------------------------------------------------------------------------------------
    FDopplerModulationOperator::FDopplerModulationOperator(
        const FOperatorSettings& InSettings,
        const FAudioBufferReadRef& InAudioInput,
        const FAudioBufferReadRef& InModulationInput,
        const FFloatReadRef& InDelayFeedbackInput,
        const FFloatReadRef& InModulationFeedbackInput,
        const FFloatReadRef& InDelayTimeInput,
        const FBoolReadRef& InInvertModulationSignalInput,
        const FFloatReadRef& InMaxSlopeInput
    )
        : AudioInput(InAudioInput)
        , ModulationInput(InModulationInput)
        , DelayFeedbackInput(InDelayFeedbackInput)
        , ModulationFeedbackInput(InModulationFeedbackInput)
        , DelayTimeInput(InDelayTimeInput)
        , InvertModulationSignalInput(InInvertModulationSignalInput)
        , MaxSlopeInput(InMaxSlopeInput)
        , AudioOutput(FAudioBufferWriteRef::CreateNew(InSettings))
    {
        const float SampleRate            = InSettings.GetSampleRate();
        constexpr float SmoothingTimeInMs = 21.33f;
        constexpr float DelaySmoothingFactor = 0.7f;
        
        DopplerModulationDSPProcessor.InitDelayFeedbackParamSmoothing(SmoothingTimeInMs, SampleRate);
        DopplerModulationDSPProcessor.InitModulationFeedbackParamSmoothing(SmoothingTimeInMs, SampleRate);
        DopplerModulationDSPProcessor.InitMaxSlopeParamSmoothing(SmoothingTimeInMs, SampleRate);
        
        DopplerModulationDSPProcessor.InitDelayBuffer(MaxDelayTimeSeconds,DelaySmoothingFactor, SampleRate);
    };

    FDataReferenceCollection FDopplerModulationOperator::GetInputs() const
    {
        using namespace DopplerModulationNode;

        FDataReferenceCollection InputDataReferences;

        InputDataReferences.AddDataReadReference(METASOUND_GET_PARAM_NAME(InParamNameAudioInput), AudioInput);
        InputDataReferences.AddDataReadReference(METASOUND_GET_PARAM_NAME(InParamNameModulationInput), ModulationInput);
        InputDataReferences.AddDataReadReference(METASOUND_GET_PARAM_NAME(InParamNameDelayFeedbackInput), DelayFeedbackInput);
        InputDataReferences.AddDataReadReference(METASOUND_GET_PARAM_NAME(InParamNameModulationFeedbackInput), ModulationFeedbackInput);
        InputDataReferences.AddDataReadReference(METASOUND_GET_PARAM_NAME(InParamNameDelayTimeInput), DelayTimeInput);
        InputDataReferences.AddDataReadReference(METASOUND_GET_PARAM_NAME(InParamNameInvertModulationSignalInput), InvertModulationSignalInput);
        InputDataReferences.AddDataReadReference(METASOUND_GET_PARAM_NAME(InParamNameMaxSlopeInput), MaxSlopeInput);

     


        return InputDataReferences;
    }

    FDataReferenceCollection FDopplerModulationOperator::GetOutputs() const
    {
        using namespace DopplerModulationNode;

        FDataReferenceCollection OutputDataReferences;

        OutputDataReferences.AddDataReadReference(METASOUND_GET_PARAM_NAME(OutParamNameAudio), AudioOutput);

        return OutputDataReferences;
    }



    const FVertexInterface& FDopplerModulationOperator::GetVertexInterface()
    {
        using namespace DopplerModulationNode;

        static const FVertexInterface Interface(
            FInputVertexInterface(
                TInputDataVertex<FAudioBuffer>(METASOUND_GET_PARAM_NAME_AND_METADATA(InParamNameAudioInput)),
                TInputDataVertex<FAudioBuffer>(METASOUND_GET_PARAM_NAME_AND_METADATA(InParamNameModulationInput)),
                TInputDataVertex<float>(METASOUND_GET_PARAM_NAME_AND_METADATA(InParamNameDelayFeedbackInput), 0.0f),
                TInputDataVertex<float>(METASOUND_GET_PARAM_NAME_AND_METADATA(InParamNameModulationFeedbackInput), 0.0f),
                TInputDataVertex<float>(METASOUND_GET_PARAM_NAME_AND_METADATA(InParamNameDelayTimeInput), 1.0f),
                TInputDataVertex<bool>(METASOUND_GET_PARAM_NAME_AND_METADATA(InParamNameInvertModulationSignalInput), 0.0f),
                TInputDataVertex<float>(METASOUND_GET_PARAM_NAME_AND_METADATA(InParamNameMaxSlopeInput), 1.0f)

            ),

            FOutputVertexInterface(
                TOutputDataVertex<FAudioBuffer>(METASOUND_GET_PARAM_NAME_AND_METADATA(OutParamNameAudio))
            )
        );

        return Interface;
    }

    const FNodeClassMetadata& FDopplerModulationOperator::GetNodeInfo()
    {
        auto InitNodeInfo = []() -> FNodeClassMetadata
        {
            FNodeClassMetadata Info;

            Info.ClassName        = { TEXT("UE"), TEXT("DopplerModulation"), TEXT("Audio") };
            Info.MajorVersion     = 1;
            Info.MinorVersion     = 0;
            Info.DisplayName      = LOCTEXT("Metasound_DopplerModulationDisplayName", "DopplerModulation");
            Info.Description      = LOCTEXT("Metasound_DopplerModulationNodeDescription", "Applies DopplerModulation to the audio input.");
            Info.Author           = PluginAuthor;
            Info.PromptIfMissing  = PluginNodeMissingPrompt;
            Info.DefaultInterface = GetVertexInterface();
            Info.CategoryHierarchy = { LOCTEXT("Metasound_DopplerModulationNodeCategory", "Utils") };

            return Info;
        };

        static const FNodeClassMetadata Info = InitNodeInfo();

        return Info;
    }

    TUniquePtr<IOperator> FDopplerModulationOperator::CreateOperator(const FCreateOperatorParams& InParams, FBuildErrorArray& OutErrors)
    {
        using namespace DopplerModulationNode;

        const FDataReferenceCollection& InputCollection = InParams.InputDataReferences;
        const FInputVertexInterface& InputInterface     = GetVertexInterface().GetInputInterface();



        FAudioBufferReadRef AudioIn = InputCollection.GetDataReadReferenceOrConstruct<FAudioBuffer>(METASOUND_GET_PARAM_NAME(InParamNameAudioInput), InParams.OperatorSettings);
        FAudioBufferReadRef ModulationIn = InputCollection.GetDataReadReferenceOrConstruct<FAudioBuffer>(METASOUND_GET_PARAM_NAME(InParamNameModulationInput), InParams.OperatorSettings);
        
        FFloatReadRef DelayFeedbackIn = InputCollection.GetDataReadReferenceOrConstructWithVertexDefault<float>(InputInterface, METASOUND_GET_PARAM_NAME(InParamNameDelayFeedbackInput), InParams.OperatorSettings);
        FFloatReadRef ModulationFeedbackIn = InputCollection.GetDataReadReferenceOrConstructWithVertexDefault<float>(InputInterface, METASOUND_GET_PARAM_NAME(InParamNameModulationFeedbackInput), InParams.OperatorSettings);
        FFloatReadRef DelayTimeIn = InputCollection.GetDataReadReferenceOrConstructWithVertexDefault<float>(InputInterface, METASOUND_GET_PARAM_NAME(InParamNameDelayTimeInput), InParams.OperatorSettings);
        FBoolReadRef InvertModulationIn = InputCollection.GetDataReadReferenceOrConstructWithVertexDefault<bool>(InputInterface, METASOUND_GET_PARAM_NAME(InParamNameInvertModulationSignalInput), InParams.OperatorSettings);
        FFloatReadRef MaxSlopeIn = InputCollection.GetDataReadReferenceOrConstructWithVertexDefault<float>(InputInterface, METASOUND_GET_PARAM_NAME(InParamNameMaxSlopeInput), InParams.OperatorSettings);

        //The DSP operations are done by a seperate class so we can share the operations between different implementation types.
        return MakeUnique<FDopplerModulationOperator>(InParams.OperatorSettings, AudioIn, ModulationIn, DelayFeedbackIn, ModulationFeedbackIn, DelayTimeIn, InvertModulationIn, MaxSlopeIn);
    }


    //------------------------------------------------------------------------------------
    // FDopplerModulationNode
    //------------------------------------------------------------------------------------
    FDopplerModulationNode::FDopplerModulationNode(const FNodeInitData& InitData)
        : FNodeFacade(InitData.InstanceName, InitData.InstanceID, TFacadeOperatorClass<FDopplerModulationOperator>())
    {
    
    }
    void FDopplerModulationOperator::Execute()
    {
        /** Audio buffer values. */
        const float* InputAudio{ AudioInput->GetData() };
        const float* InputModulation{ ModulationInput->GetData() };
        float* OutputAudio{ AudioOutput->GetData() };


        /** Parameters. */
        float DelayFeedbackClamped{ FMath::Clamp(*DelayFeedbackInput, 0.0f, 1.0f - SMALL_NUMBER) };
        float ModulationFeedbackClamped{ FMath::Clamp(*ModulationFeedbackInput, 0.0f, 1.0f - SMALL_NUMBER) };
        float DelayTimeClamped{ FMath::Clamp(*DelayTimeInput, DopplerModulationNode::MinMaxDelaySeconds, DopplerModulationNode::MaxMaxDelaySeconds ) }; //TODO: GetMaxDelayTime.
        bool InvertModulationSignal{ *InvertModulationSignalInput };
        float MaxSlope {*MaxSlopeInput};

        const int32 NumSamples = AudioInput->Num();
        //DopplerModulationDSPProcessor.SetParameters(DelayFeedbackClamped, ModulationFeedbackClamped, DelayTimeClamped, InvertModulationSignal,MaxSlope);
        DopplerModulationDSPProcessor.SetDelayFeedback(DelayFeedbackClamped);
        DopplerModulationDSPProcessor.SetModulationFeedback(ModulationFeedbackClamped);
        DopplerModulationDSPProcessor.SetDelayTimeSeconds(DelayTimeClamped);
        DopplerModulationDSPProcessor.SetInvertModulationSignal(InvertModulationSignal);
        DopplerModulationDSPProcessor.SetMaxSlope(MaxSlope);
        
        
        DopplerModulationDSPProcessor.ProcessAudioBuffer(InputAudio,InputModulation, OutputAudio, NumSamples);
    }

    METASOUND_REGISTER_NODE(FDopplerModulationNode)
}

#undef LOCTEXT_NAMESPACE
