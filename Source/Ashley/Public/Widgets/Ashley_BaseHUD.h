// Ashley

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "Widgets/Ashley_InfoText.h"
#include "Widgets/Ashley_ChoiceMenu.h"
#include "Widgets/Ashley_SkipCutscene.h"

#include "Ashley_BaseHUD.generated.h"

/**
 * 
 */
UCLASS()
class ASHLEY_API UAshley_BaseHUD : public UUserWidget
{
	GENERATED_BODY()

public:

	void NativePreConstruct() override;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UAshley_InfoText* DialogueText;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UAshley_ChoiceMenu* DialogueChoices;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UAshley_InfoText* InteractionPrompt;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UAshley_InfoText* HintPrompt;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UAshley_SkipCutscene* SkipCutscenePrompt;
};
