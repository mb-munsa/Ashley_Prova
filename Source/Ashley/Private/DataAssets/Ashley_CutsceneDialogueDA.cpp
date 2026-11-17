// Ashley

#include "DataAssets/Ashley_CutsceneDialogueDA.h"
#include <Managers/Ashley_WidgetSystem.h>

void UAshley_CutsceneDialogueDA::PlayDialogue(UGameInstance* GameInstance, int SequenceIndex, int DialogueIndex)
{
	if (!Sequences.Find(SequenceIndex) || !Sequences[SequenceIndex].Dialogues.IsValidIndex(DialogueIndex))
        return;

    auto& Dialogue = Sequences[SequenceIndex].Dialogues[DialogueIndex];

    GameInstance->GetSubsystem<UAshley_WidgetSystem>()->ShowDialogue(Dialogue);
}
