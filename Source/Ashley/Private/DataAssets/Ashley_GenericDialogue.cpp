// Ashley


#include "DataAssets/Ashley_GenericDialogue.h"
#include "Managers/Ashley_WidgetSystem.h"


// TODO: refactor?

// Aggiungere play loop in range (magari random)

void UAshley_GenericDialogue::PlayDialogue(UGameInstance* GameInstance, int DialogueIndex)
{
	PlayDialogue(GameInstance, DialogueIndex, FSimpleDelegate());
}

void UAshley_GenericDialogue::PlayDialogue(UGameInstance* GameInstance, int DialogueIndex, FSimpleDelegate OnDialogueEnded)
{
	if (DialogueIndex != -1)
		LastDialogueIndex = DialogueIndex < Dialogues.Num() ? DialogueIndex : 0;

	auto& Dialogue = Dialogues[LastDialogueIndex];

	GameInstance->GetSubsystem<UAshley_WidgetSystem>()->ShowDialogue(Dialogue, MoveTemp(OnDialogueEnded));

	LastDialogueIndex = (LastDialogueIndex + 1) % Dialogues.Num();
}

void UAshley_GenericDialogue::PlayDialogueWithCallback(UGameInstance* GameInstance, int DialogueIndex, const FOnGenericDialogueEnded& OnDialogueEnded)
{
	// Il delegate dinamico tiene un riferimento weak all'oggetto, se viene distrutto il callback viene saltato
	PlayDialogue(GameInstance, DialogueIndex, FSimpleDelegate::CreateLambda([OnDialogueEnded]()
	{
		OnDialogueEnded.ExecuteIfBound();
	}));
}

void UAshley_GenericDialogue::PlayGenericDialogue(UGameInstance* GameInstance, int DialogueIndex)
{
	if (DialogueIndex >= GenericDialogues.Num())
		return;

	GenericDialogues[DialogueIndex]->PlayDialogue(GameInstance, -1);
}
