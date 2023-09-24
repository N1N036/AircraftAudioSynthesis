#include "MetasoundNodes/MetasoundPowerNode.h"
#include "MetasoundOperatorSettings.h"

#define LOCTEXT_NAMESPACE "MetasoundStandardNodes_PowerNode"

namespace Metasound
{
    //------------------------------------------------------------------------------------
    // FPowerOperator
    //------------------------------------------------------------------------------------
    FPowerOperator::FPowerOperator(const FOperatorSettings& InSettings, const FAudioBufferReadRef& InAudioInput, const FFloatReadRef& InPower)
        : AudioInput(InAudioInput)
        , AudioOutput(FAudioBufferWriteRef::CreateNew(InSettings))
        , Power(InPower)
    {
        PowerDSPProcessor.Init();
    }

    FDataReferenceCollection FPowerOperator::GetInputs() const
    {
        using namespace PowerNode;

        FDataReferenceCollection InputDataReferences;

        InputDataReferences.AddDataReadReference(METASOUND_GET_PARAM_NAME(InParamNameAudioInput), AudioInput);
        InputDataReferences.AddDataReadReference(METASOUND_GET_PARAM_NAME(InParamNamePower), Power);

        return InputDataReferences;
    }

    FDataReferenceCollection FPowerOperator::GetOutputs() const
    {
        using namespace PowerNode;

        FDataReferenceCollection OutputDataReferences;

        OutputDataReferences.AddDataReadReference(METASOUND_GET_PARAM_NAME(OutParamNameAudio), AudioOutput);

        return OutputDataReferences;
    }

    void FPowerOperator::Execute()
    {
        const float* InputAudio = AudioInput->GetData();
        float* OutputAudio      = AudioOutput->GetData();

        const int32 NumSamples = AudioInput->Num();

        PowerDSPProcessor.SetPower(*Power);

        PowerDSPProcessor.ProcessAudioBuffer(InputAudio, OutputAudio, NumSamples);
    }

    const FVertexInterface& FPowerOperator::GetVertexInterface()
    {
        using namespace PowerNode;

        static const FVertexInterface Interface(
            FInputVertexInterface(
                TInputDataVertex<FAudioBuffer>(METASOUND_GET_PARAM_NAME_AND_METADATA(InParamNameAudioInput)),
                TInputDataVertex<float>(METASOUND_GET_PARAM_NAME_AND_METADATA(InParamNamePower), 1.0f)
            ),

            FOutputVertexInterface(
                TOutputDataVertex<FAudioBuffer>(METASOUND_GET_PARAM_NAME_AND_METADATA(OutParamNameAudio))
            )
        );

        return Interface;
    }

    const FNodeClassMetadata& FPowerOperator::GetNodeInfo()
    {
        auto InitNodeInfo = []() -> FNodeClassMetadata
        {
            FNodeClassMetadata Info;

            Info.ClassName        = { TEXT("UE"), TEXT("Power"), TEXT("Audio") };
            Info.MajorVersion     = 1;
            Info.MinorVersion     = 0;
            Info.DisplayName      = LOCTEXT("Metasound_PowerDisplayName", "Power");
            Info.Description      = LOCTEXT("Metasound_PowerNodeDescription", "Applies power to the audio input.");
            Info.Author           = PluginAuthor;
            Info.PromptIfMissing  = PluginNodeMissingPrompt;
            Info.DefaultInterface = GetVertexInterface();
            Info.CategoryHierarchy = { LOCTEXT("Metasound_PowerNodeCategory", "Utils") };

            return Info;
        };

        static const FNodeClassMetadata Info = InitNodeInfo();

        return Info;
    }

    TUniquePtr<IOperator> FPowerOperator::CreateOperator(const FCreateOperatorParams& InParams, FBuildErrorArray& OutErrors)
    {
        using namespace PowerNode;

        const FDataReferenceCollection& InputCollection = InParams.InputDataReferences;
        const FInputVertexInterface& InputInterface     = GetVertexInterface().GetInputInterface();

        FAudioBufferReadRef AudioIn = InputCollection.GetDataReadReferenceOrConstruct<FAudioBuffer>(METASOUND_GET_PARAM_NAME(InParamNameAudioInput), InParams.OperatorSettings);
        FFloatReadRef InPower   = InputCollection.GetDataReadReferenceOrConstructWithVertexDefault<float>(InputInterface, METASOUND_GET_PARAM_NAME(InParamNamePower), InParams.OperatorSettings);

        //The DSP operations are done by a seperate class so we can share the operations between different implementation types.
        return MakeUnique<FPowerOperator>(InParams.OperatorSettings, AudioIn, InPower);
    }


    //------------------------------------------------------------------------------------
    // FPowerNode
    //------------------------------------------------------------------------------------
    FPowerNode::FPowerNode(const FNodeInitData& InitData)
        : FNodeFacade(InitData.InstanceName, InitData.InstanceID, TFacadeOperatorClass<FPowerOperator>())
    {
    
    }

    METASOUND_REGISTER_NODE(FPowerNode)
}

#undef LOCTEXT_NAMESPACE
