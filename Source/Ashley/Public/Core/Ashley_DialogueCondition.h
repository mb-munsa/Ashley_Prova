// Ashley

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Ashley_DialogueCondition.generated.h"

/**
 * 
 */
UCLASS(Abstract, BlueprintType, EditInlineNew)
class ASHLEY_API UAshley_DialogueCondition : public UObject
{
	GENERATED_BODY()

public:
    UFUNCTION(BlueprintNativeEvent)
    bool Evaluate(const UObject* WorldContext) const;
	
};
