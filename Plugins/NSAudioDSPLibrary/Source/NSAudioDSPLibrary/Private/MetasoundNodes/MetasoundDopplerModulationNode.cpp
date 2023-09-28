#include "MetasoundNodes/MetasoundDopplerModulationNode.h"
#include "MetasoundOperatorSettings.h"

#define LOCTEXT_NAMESPACE "MetasoundStandardNodes_DopplerModulationNode"

namespace Metasound
{
    //------------------------------------------------------------------------------------
    // FDopplerModulationOperator
    //------------------------------------------------------------------------------------
    FDopplerModulationOperator::FDopplerModulationOperator(const FOperatorSettings& InSettings, const FAudioBufferReadRef& InAudioInput, const FAudioBufferReadRef& InModulationInput)
        : AudioInput(InAudioInput)
        , ModulationInput(InModulationInput)
        , AudioOutput(FAudioBufferWriteRef::CreateNew(InSettings))
    {
        DopplerModulationDSPProcessor.Init(InSettings.GetSampleRate(), 0.1); //TODO: add the delay time as stateless input. 
    }

    FDataReferenceCollection FDopplerModulationOperator::GetInputs() const
    {
        using namespace DopplerModulationNode;

        FDataReferenceCollection InputDataReferences;

        InputDataReferences.AddDataReadReference(METASOUND_GET_PARAM_NAME(InParamNameAudioInput), AudioInput);
        InputDataReferences.AddDataReadReference(METASOUND_GET_PARAM_NAME(InParamNameModulationInput), ModulationInput);

        return InputDataReferences;
    }

    FDataReferenceCollection FDopplerModulationOperator::GetOutputs() const
    {
        using namespace DopplerModulationNode;

        FDataReferenceCollection OutputDataReferences;

        OutputDataReferences.AddDataReadReference(METASOUND_GET_PARAM_NAME(OutParamNameAudio), AudioOutput);

        return OutputDataReferences;
    }

    void FDopplerModulationOperator::Execute()
    {
        const float* InputAudio = AudioInput->GetData();
        const float* InputModulation = ModulationInput->GetData();
        float* OutputAudio      = AudioOutput->GetData();

        const int32 NumSamples = AudioInput->Num();

        if(!InputAudio || !OutputAudio){return;}
        DopplerModulationDSPProcessor.SetParameters(1,1,1,1);

        DopplerModulationDSPProcessor.ProcessAudioBuffer(InputAudio,InputModulation, OutputAudio, NumSamples); //TODO: for now i just put the input audio as modulation, a new channel needs to be added later
    }

    const FVertexInterface& FDopplerModulationOperator::GetVertexInterface()
    {
        using namespace DopplerModulationNode;

        static const FVertexInterface Interface(
            FInputVertexInterface(
                TInputDataVertex<FAudioBuffer>(METASOUND_GET_PARAM_NAME_AND_METADATA(InParamNameAudioInput)),
                TInputDataVertex<FAudioBuffer>(METASOUND_GET_PARAM_NAME_AND_METADATA(InParamNameModulationInput))
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

        //The DSP operations are done by a seperate class so we can share the operations between different implementation types.
        return MakeUnique<FDopplerModulationOperator>(InParams.OperatorSettings, AudioIn, ModulationIn);
    }


    //------------------------------------------------------------------------------------
    // FDopplerModulationNode
    //------------------------------------------------------------------------------------
    FDopplerModulationNode::FDopplerModulationNode(const FNodeInitData& InitData)
        : FNodeFacade(InitData.InstanceName, InitData.InstanceID, TFacadeOperatorClass<FDopplerModulationOperator>())
    {
    
    }

    METASOUND_REGISTER_NODE(FDopplerModulationNode)
}

#undef LOCTEXT_NAMESPACE
