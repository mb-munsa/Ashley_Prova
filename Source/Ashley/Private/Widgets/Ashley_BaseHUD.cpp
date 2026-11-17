// Ashley


#include "Widgets/Ashley_BaseHUD.h"

void UAshley_BaseHUD::NativePreConstruct()
{
    Super::NativePreConstruct();

    InteractionPrompt->SetVisibility(ESlateVisibility::Collapsed);
    DialogueText->SetVisibility(ESlateVisibility::Collapsed);
    SkipCutscenePrompt->SetVisibility(ESlateVisibility::Collapsed);
    HintPrompt->SetVisibility(ESlateVisibility::Collapsed);
}