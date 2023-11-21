#include "DSPProcessing/DopplerModulation.h"
#include "MetasoundEnumRegistrationMacro.h"
#include "MetasoundParamHelper.h"
#include "MetasoundTime.h"
#include "MetasoundPrimitives.h"
#include "MetasoundOperatorInterface.h"


namespace Metasound
{
#define LOCTEXT_NAMESPACE "MetasoundStandardNodes_DopplerModulationNode"

	namespace DopplerModulationNode
	{
		METASOUND_PARAM(InParamNameAudioInput, "In", "Audio input.")
		METASOUND_PARAM(InParamNameModulationInput, "Modulation", "The delay modulation signal.")

		METASOUND_PARAM(InParamNameDelayFeedbackInput, "DelayFeedback", "How much the output signal will be fed back into the inpot signal.")
		METASOUND_PARAM(InParamNameModulationFeedbackInput, "ModulationFeedback", "How much the output signal will be fed back into the modulation signal.")
		METASOUND_PARAM(InParamNameDelayTimeInput, "DelayTime", "How long the delay sould be, the modulation modulates from no delay to this delay time.")
		METASOUND_PARAM(InParamNameInvertModulationSignalInput, "InvertModulation", "Flip the modulation signal for a variation of the modulation feedback effect.")
		METASOUND_PARAM(InParamNameModlationLowPassInput, "ModulationLowPass", "Low pass the modulation signal.")
		METASOUND_PARAM(InParamNameModlationHighPassInput, "ModulationHighPass", "High pass the modulation signal.")
	
		

			
			METASOUND_PARAM(OutParamNameAudio, "Out","Audio output.")

		static constexpr float MinMaxDelaySeconds = 0.001f;
		static constexpr float MaxMaxDelaySeconds = 1000.f;
		static constexpr float DefaultMaxDelaySeconds = 1.0f;
	}

#undef LOCTEXT_NAMESPACE
	class FDopplerModulationOperator : public TExecutableOperator<FDopplerModulationOperator>
	{
	public:
		static const FNodeClassMetadata& GetNodeInfo();
		static const FVertexInterface& GetVertexInterface();
		static TUniquePtr<IOperator> CreateOperator(const FCreateOperatorParams& InParams, FBuildErrorArray& OutErrors);

		FDopplerModulationOperator(
			const FOperatorSettings& InSettings,
			const FAudioBufferReadRef& InAudioInput,
			const FAudioBufferReadRef& InModulationInput,
			const FFloatReadRef& InDelayFeedbackInput,
			const FFloatReadRef& InModulationFeedbackInput,
			const FFloatReadRef& InDelayTimeInput,
			const FBoolReadRef& InInvertModulationSignalInput,
			const FFloatReadRef& InModulationLowPassInput,
			const FFloatReadRef& InModulationHighPassInput
		);

		virtual FDataReferenceCollection GetInputs()  const override;
		virtual FDataReferenceCollection GetOutputs() const override;

		void Execute();

	private:
		/**Audio signals */
		FAudioBufferReadRef AudioInput;
		FAudioBufferReadRef ModulationInput;
		FAudioBufferWriteRef AudioOutput;

		/**Parameter input. */
		FFloatReadRef DelayFeedbackInput;
		FFloatReadRef ModulationFeedbackInput;
		FFloatReadRef DelayTimeInput;
		FBoolReadRef InvertModulationSignalInput;
		FFloatReadRef ModulationLowPass;
		FFloatReadRef ModulationHighPass;
		
		
		float MaxDelayTimeSeconds{ 1.0f }; //TODO: figure out how to get the max delay value from Delay::"

		// The internal delay buffer
		Audio::FDelay DelayBuffer;

		//The effect processor using a delay buffer.
		DSPProcessing::FDopplerModulation DopplerModulationDSPProcessor;

	};
	class FDopplerModulationNode : public FNodeFacade
	{
	public:
		FDopplerModulationNode(const FNodeInitData& InitData);
	};
}
