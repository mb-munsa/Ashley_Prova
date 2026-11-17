// Ashley

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Tags_Enums_Structs/Ashley_MenuPickusStructs.h"

#include "Ashley_InteractInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UAshley_InteractInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class ASHLEY_API IAshley_InteractInterface
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintNativeEvent, Category = "Interaction")
	void Interact();

	UFUNCTION()
	virtual bool IsInteractable() const { return true; }
	
	virtual void ShowInteractionPrompt() const {};
	virtual void HideInteractionPrompt() const {};

	virtual FString GetInteractMessage() const { return "Interact"; }
};
