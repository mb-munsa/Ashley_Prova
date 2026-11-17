 // Ashley

#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputComponent.h"
#include "DataAssets/Ashley_InputConfigDA.h"

#include "Ashley_InputComponent.generated.h"

/**
 * 
 */
UCLASS()
class ASHLEY_API UAshley_InputComponent : public UEnhancedInputComponent
{
	GENERATED_BODY()
	
public:

	template<class UserObject, typename CallbackFunc>
	void BindNativeInputAction(const UAshley_InputConfigDA* InInputConfig, UPARAM(meta = (Categories = "Ashley.InputAction")) const FGameplayTag& InInputTag, ETriggerEvent TriggerEvent, UserObject* ContextObject, CallbackFunc Func);

};

template<class UserObject, typename CallbackFunc>
inline void UAshley_InputComponent::BindNativeInputAction(const UAshley_InputConfigDA* InInputConfig, UPARAM(meta = (Categories = "House.InputAction")) const FGameplayTag& InInputTag, ETriggerEvent TriggerEvent, UserObject* ContextObject, CallbackFunc Func)
{
	checkf(InInputConfig, TEXT("Input config data asset is null,can not proceed with binding"));
	if (UInputAction* FoundAction = InInputConfig->FindNativeInputActionByTag(InInputTag))
	{
		BindAction(FoundAction, TriggerEvent, ContextObject, Func);
	}
}