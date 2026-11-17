// Ashley


#include "Core/Ashley_DialogueComponent.h"
#include "Core/Ashley_DialogueCondition.h"

#include "Components/AudioComponent.h"

#include "DataAssets/Ashley_DialogueNode.h"
#include "Widgets/Ashley_ChoiceMenu.h"
#include "Managers/Ashley_WidgetSystem.h"

#include "DebugHelper.h"

#include <Kismet/GameplayStatics.h>

// Sets default values for this component's properties
UAshley_DialogueComponent::UAshley_DialogueComponent()
{
}

void UAshley_DialogueComponent::OnRegister()
{
	Super::OnRegister();

	AActor* MyOwner = GetOwner();

	if (MyOwner)
	{
		MyAudioComponent = NewObject<UAudioComponent>(MyOwner, TEXT("DialogueAudioSource"));
		MyAudioComponent->RegisterComponent();

		// Optional: if you want it to persist/save with the actor instance
		// (e.g. visible in the editor after running construction scripts):
		//MyOwner->AddInstanceComponent(NewComp);

		if (!MyAudioComponent) Debug::Print("ERRORE - AUDIO SOURCE MANCANTE IN DIALOGUE COMPONENT ACTOR!");
	}
}

bool UAshley_DialogueComponent::IsChoiceAvailable(FDialogueChoice Choice)
{
	if (Choice.Conditions.IsEmpty())
		return true;

	// Se una delle condizioni non è soddisfatta, non rendo visibile la scelta
	for (auto& Condition : Choice.Conditions)
	{
		if (!Condition->Evaluate(this)) return false;
	}

	return true;
}

void UAshley_DialogueComponent::Play(UAshley_DialogueNode* DialogueNode)
{
	if (!WidgetManager)
		WidgetManager = UGameplayStatics::GetGameInstance(this)->GetSubsystem<UAshley_WidgetSystem>();

	CurrentDialogueNode = DialogueNode;
	bIsDialogueOngoing = false;

	// Sono arrivato alla fine del flusso di dialogo
	if (!CurrentDialogueNode)
	{
		//Debug::Print("Dialogue over!");

		WidgetManager->EndDialogue();
		OnDialogueEnded.Broadcast();
		return;
	}

	// Se è un dialogueNode con scelte, le propongo
	if (DialogueNode->bIsChoiceMenu)
	{
		//Debug::Print("Is choice Menu!");

		TArray<FDialogueChoice> ChoicesToShow;

		// Mostro solo le scelte ancora valide
		for (int i = 0; i < DialogueNode->DialogueChoices.Num(); i++)
		{
			auto& Choice = DialogueNode->DialogueChoices[i];
			if ((!Choice.bIsOneShot || !ChoicesVisited.Contains({ DialogueNode->NodeID, i }))
				&& IsChoiceAvailable(Choice))
				ChoicesToShow.Add(Choice);
		}

		ChoiceMenu = WidgetManager->ShowDialogueChoices(ChoicesToShow);

		// Mi collego all'evento di callback di scelta
		ChoiceMenu->OnChoiceSelected.AddDynamic(this, &UAshley_DialogueComponent::OnChoiceSelected);
	}
	// Altrimenti comincio a mostrare le linee di dialogo
	else
	{
		//Debug::Print("Starting dialogue lines..");
		bIsDialogueOngoing = true;

		CurrentLineIndex = 0;

		PlayDialogueLine(CurrentLineIndex);
	}
}

void UAshley_DialogueComponent::PlayDialogueLine(int LineIndex)
{
	// Se sono arrivato all'ultima linea, passo al nodo successivo
	if (LineIndex >= CurrentDialogueNode->Dialogue.DialogueLines.Num())
	{
		//Debug::Print("LINES OVER");

		this->Play(CurrentDialogueNode->FollowingNode);
		return;
	}

	auto& CurrDialogueLine = CurrentDialogueNode->Dialogue.DialogueLines[LineIndex];
	float CurrLineDuration = CurrDialogueLine.DialogueDuration;

	if (CurrDialogueLine.DialogueVoiceover)
	{
		//Debug::Print("VOICEOVER");

		// Sovrascrivo duration in base a voiceOver
		CurrLineDuration = CurrDialogueLine.DialogueVoiceover->GetDuration();
		MyAudioComponent->SetSound(CurrDialogueLine.DialogueVoiceover);
		MyAudioComponent->Play();
	}

	// Estraggo il nome del personaggio
	FString TagString = CurrDialogueLine.CharacterSpeaking.ToString();
	FString CharacterName;

	// Mostro il testo preceduto dal nome personaggio
	TagString.Split(TEXT("."), nullptr, &CharacterName, ESearchCase::IgnoreCase, ESearchDir::FromEnd);
	FString Result = CharacterName + TEXT(": ") + CurrDialogueLine.Dialogue;

	WidgetManager->SpawnInfoText(Result, -1);

	CurrentLineIndex++;

	GetWorld()->GetTimerManager().SetTimer(
		TimerHandle_NextLine,
		[this]() { this->PlayDialogueLine(CurrentLineIndex); },
		CurrLineDuration,
		false
	);
}

void UAshley_DialogueComponent::SkipToNextLine()
{
	if (!bIsDialogueOngoing) return;

	MyAudioComponent->Stop();
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle_NextLine);
	PlayDialogueLine(CurrentLineIndex);
}

void UAshley_DialogueComponent::OnChoiceSelected(int ChoiceIndex)
{
	//Debug::Print("On choice selected");
	if (ChoiceIndex >= CurrentDialogueNode->DialogueChoices.Num())
	{
		//Debug::Print("Invalide CHOICE selected");
		return;
	}

	ChoicesVisited.Add({ CurrentDialogueNode->NodeID, ChoiceIndex });

	OnChoiceVisited.Broadcast(ChoiceIndex);

	WidgetManager->HideChoiceMenu();
	ChoiceMenu->OnChoiceSelected.RemoveDynamic(this, &UAshley_DialogueComponent::OnChoiceSelected);

	auto NextDialogue = CurrentDialogueNode->DialogueChoices[ChoiceIndex].FollowingNode;
	this->Play(NextDialogue);
}

