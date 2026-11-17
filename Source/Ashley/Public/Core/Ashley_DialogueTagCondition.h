// Ashley

#pragma once

#include "CoreMinimal.h"
#include "NativeGameplayTags.h"

#include "Core/Ashley_DialogueCondition.h"
#include "Ashley_DialogueTagCondition.generated.h"

/**
 * 
 */
UCLASS(BlueprintType, EditInlineNew)
class ASHLEY_API UAshley_DialogueTagCondition : public UAshley_DialogueCondition
{
    GENERATED_BODY()

public:

    UPROPERTY(EditAnywhere)
    FGameplayTag Tag;

    UPROPERTY(EditAnywhere)
    bool bExpectedValue = true;

    virtual bool Evaluate_Implementation(const UObject* WorldContext) const override;
	
};
