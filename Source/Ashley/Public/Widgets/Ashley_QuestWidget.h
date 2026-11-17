// Ashley

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Ashley_QuestWidget.generated.h"

class UTextBlock;

/**
 * 
 */
UCLASS()
class ASHLEY_API UAshley_QuestWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	UFUNCTION()
	void SetQuestName(const FString& TextToDisplay);
	void SetQuestText(const FString& TextToDisplay);

protected:

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* QuestName;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* QuestText;
};
