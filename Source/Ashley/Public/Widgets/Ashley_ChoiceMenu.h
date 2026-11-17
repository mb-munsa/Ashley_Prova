// Ashley

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "DataAssets/Ashley_DialogueNode.h"

#include "Ashley_ChoiceMenu.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnChoiceSelected, int, ChoiceIndex);

/**
 * 
 */
UCLASS()
class ASHLEY_API UAshley_ChoiceMenu : public UUserWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintCallable)
	FOnChoiceSelected OnChoiceSelected;

	UFUNCTION(BlueprintImplementableEvent)
	void ShowChoices(const TArray<FDialogueChoice>& ChoicesToShow);

};
