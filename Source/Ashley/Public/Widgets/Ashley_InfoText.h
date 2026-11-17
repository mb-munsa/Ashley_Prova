// Ashley

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Ashley_InfoText.generated.h"

/**
 * 
 */
UCLASS()
class ASHLEY_API UAshley_InfoText : public UUserWidget
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintImplementableEvent)
	void SetTextToDisplay(const FString& TextToDisplay);
	
};
