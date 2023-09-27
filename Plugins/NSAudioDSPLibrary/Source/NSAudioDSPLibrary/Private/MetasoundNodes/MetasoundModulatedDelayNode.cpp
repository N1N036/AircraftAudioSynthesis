#include "MetasoundNodes/MetasoundModulatedDelayNode.h"
#include "MetasoundOperatorSettings.h"

#define LOCTEXT_NAMESPACE "MetasoundStandardNodes_ModulatedDelayNode"

namespace Metasound
{
    //------------------------------------------------------------------------------------
    // FModulatedDelayOperator
    //------------------------------------------------------------------------------------
    FModulatedDelayOperator::FModulatedDelayOperator(const FOperatorSettings& InSettings, const FAudioBufferReadRef& InAudioInput, const FAudioBufferReadRef& InModulationInput)
        : AudioInput(InAudioInput)
        , ModulationInput(InModulationInput)
        , AudioOutput(FAudioBufferWriteRef::CreateNew(InSettings))
    {
        ModulatedDelayDSPProcessor.Init(InSettings.GetSampleRate(), 0.1); //TODO: add the delay time as stateless input. 
    }

    FDataReferenceCollection FModulatedDelayOperator::GetInputs() const
    {
        using namespace ModulatedDelayNode;

        FDataReferenceCollection InputDataReferences;

        InputDataReferences.AddDataReadReference(METASOUND_GET_PARAM_NAME(InParamNameAudioInput), AudioInput);
        InputDataReferences.AddDataReadReference(METASOUND_GET_PARAM_NAME(InParamNameModulationInput), ModulationInput);

        return InputDataReferences;
    }

    FDataReferenceCollection FModulatedDelayOperator::GetOutputs() const
    {
        using namespace ModulatedDelayNode;

        FDataReferenceCollection OutputDataReferences;

        OutputDataReferences.AddDataReadReference(METASOUND_GET_PARAM_NAME(OutParamNameAudio), AudioOutput);

        return OutputDataReferences;
    }

    void FModulatedDelayOperator::Execute()
    {//this line caused EXCEPTION_ACCESS_VIOLATION reading address 0x0000000000000000
        const float* InputAudio = AudioInput->GetData();
        const float* InputModulation = ModulationInput->GetData();
        //const float* InputModulation = ModulationInput->GetData(); //TODO: Use this.
        float* OutputAudio      = AudioOutput->GetData();

        const int32 NumSamples = AudioInput->Num();

        if(!InputAudio || !OutputAudio){return;}
        ModulatedDelayDSPProcessor.SetParameters(1,1,1,1);

        ModulatedDelayDSPProcessor.ProcessAudioBuffer(InputAudio,InputModulation, OutputAudio, NumSamples); //TODO: for now i just put the input audio as modulation, a new channel needs to be added later
    }

    const FVertexInterface& FModulatedDelayOperator::GetVertexInterface()
    {
        using namespace ModulatedDelayNode;

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

    const FNodeClassMetadata& FModulatedDelayOperator::GetNodeInfo()
    {
        auto InitNodeInfo = []() -> FNodeClassMetadata
        {
            FNodeClassMetadata Info;

            Info.ClassName        = { TEXT("UE"), TEXT("ModulatedDelay"), TEXT("Audio") };
            Info.MajorVersion     = 1;
            Info.MinorVersion     = 0;
            Info.DisplayName      = LOCTEXT("Metasound_ModulatedDelayDisplayName", "ModulatedDelay");
            Info.Description      = LOCTEXT("Metasound_ModulatedDelayNodeDescription", "Applies ModulatedDelay to the audio input.");
            Info.Author           = PluginAuthor;
            Info.PromptIfMissing  = PluginNodeMissingPrompt;
            Info.DefaultInterface = GetVertexInterface();
            Info.CategoryHierarchy = { LOCTEXT("Metasound_ModulatedDelayNodeCategory", "Utils") };

            return Info;
        };

        static const FNodeClassMetadata Info = InitNodeInfo();

        return Info;
    }

    TUniquePtr<IOperator> FModulatedDelayOperator::CreateOperator(const FCreateOperatorParams& InParams, FBuildErrorArray& OutErrors)
    {
        using namespace ModulatedDelayNode;

        const FDataReferenceCollection& InputCollection = InParams.InputDataReferences;
        const FInputVertexInterface& InputInterface     = GetVertexInterface().GetInputInterface();

        FAudioBufferReadRef AudioIn = InputCollection.GetDataReadReferenceOrConstruct<FAudioBuffer>(METASOUND_GET_PARAM_NAME(InParamNameAudioInput), InParams.OperatorSettings);
        FAudioBufferReadRef ModulationIn = InputCollection.GetDataReadReferenceOrConstruct<FAudioBuffer>(METASOUND_GET_PARAM_NAME(InParamNameModulationInput), InParams.OperatorSettings);

        //The DSP operations are done by a seperate class so we can share the operations between different implementation types.
        return MakeUnique<FModulatedDelayOperator>(InParams.OperatorSettings, AudioIn, ModulationIn);
    }


    //------------------------------------------------------------------------------------
    // FModulatedDelayNode
    //------------------------------------------------------------------------------------
    FModulatedDelayNode::FModulatedDelayNode(const FNodeInitData& InitData)
        : FNodeFacade(InitData.InstanceName, InitData.InstanceID, TFacadeOperatorClass<FModulatedDelayOperator>())
    {
    
    }

    METASOUND_REGISTER_NODE(FModulatedDelayNode)
}

#undef LOCTEXT_NAMESPACE
