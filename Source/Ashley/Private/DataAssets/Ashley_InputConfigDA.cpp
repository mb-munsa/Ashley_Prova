// Ashley


#include "DataAssets/Ashley_InputConfigDA.h"

UInputAction* UAshley_InputConfigDA::FindNativeInputActionByTag(const FGameplayTag& InInputTag) const
{
	for (const FAshley_InputActionConfig& InputActionConfig : NativeInputActions)
	{
		if (InputActionConfig.InputTag == InInputTag && InputActionConfig.InputAction)
		{
			return InputActionConfig.InputAction;
			break;
		}
	}

	return nullptr;
}

UInputMappingContext* UAshley_InputConfigDA::GetMappingContextByName(const EAshley_InputMappingContext& Name) const
{
	for (auto& MappingContext : MappingContexts)
	{
		if (MappingContext.InputContextName == Name)
			return MappingContext.MappingContext;
	}

	return nullptr;
}
