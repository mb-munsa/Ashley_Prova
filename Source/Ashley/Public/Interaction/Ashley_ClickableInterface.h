// Ashley

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Ashley_ClickableInterface.generated.h"


// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UAshley_ClickableInterface : public UInterface
{
	GENERATED_BODY()
	
};

/**
 *
 */
class ASHLEY_API IAshley_ClickableInterface
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintNativeEvent, Category = "Interaction")
	void Click();
};
