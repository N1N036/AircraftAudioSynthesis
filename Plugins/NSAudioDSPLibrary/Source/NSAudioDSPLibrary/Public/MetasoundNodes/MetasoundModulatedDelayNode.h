#include "DSPProcessing/Power.h"
#include "MetasoundEnumRegistrationMacro.h"
#include "MetasoundParamHelper.h"

namespace Metasound
{
#define LOCTEXT_NAMESPACE "MetasoundStandardNodes_PowerNode"

	namespace PowerNode
	{
		METASOUND_PARAM(InParamNameAudioInput, "In",        "Audio input.")
		METASOUND_PARAM(InParamNamePower,  "Power", "To which power we will amplify the input signal.")
		METASOUND_PARAM(OutParamNameAudio,     "Out",       "Audio output.")
	}

#undef LOCTEXT_NAMESPACE
	class FPowerOperator : public TExecutableOperator<FPowerOperator>
	{
	public:
		static const FNodeClassMetadata& GetNodeInfo();
		static const FVertexInterface& GetVertexInterface();
		static TUniquePtr<IOperator> CreateOperator(const FCreateOperatorParams& InParams, FBuildErrorArray& OutErrors);

		FPowerOperator(const FOperatorSettings& InSettings, const FAudioBufferReadRef& InAudioInput, const FFloatReadRef& InPower);

		virtual FDataReferenceCollection GetInputs()  const override;
		virtual FDataReferenceCollection GetOutputs() const override;

		void Execute();

	private:
		FAudioBufferReadRef  AudioInput;
		FAudioBufferWriteRef AudioOutput;

		DSPProcessing::FPower PowerDSPProcessor;

		FFloatReadRef Power;
	};
	class FPowerNode : public FNodeFacade
	{
	public:
		FPowerNode(const FNodeInitData& InitData);
	};
}
