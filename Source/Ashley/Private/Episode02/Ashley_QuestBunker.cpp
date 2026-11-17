// Ashley


#include "Episode02/Ashley_QuestBunker.h"

#include "Characters/Players/Ashley_CharacterPlayer.h"
#include "Components/Ashley_InventoryComponent.h"
#include "Managers/Ashley_QuestManager.h"
#include <Kismet/GameplayStatics.h>

void AAshley_QuestBunker::BeginPlay()
{
	Super::BeginPlay();
}

void AAshley_QuestBunker::Interact_Implementation()
{
	Super::Interact_Implementation();

	if (Character == nullptr)
		Character = Cast<AAshley_CharacterPlayer>(GetWorld()->GetFirstPlayerController()->GetCharacter());

	auto Inventory = Character->GetInventory();
	auto QuestManager = Character->GetQuestManager();

	switch (CurrentState)
	{
	case InitialState:
		if (Inventory->UseItem(ShelterKeysID))
		{
			CurrentState = NeedHammerNails;
			PlayDoorOpenAnimation();
		}
		break;
	case NeedHammerNails:
		if (Inventory->UseItem(HammerID) && Inventory->UseItem(NailsID))
			CurrentState = RepairedDoor;
		else if (Inventory->UseItem(NailsID))
			CurrentState = NeedHammer;
		else if (Inventory->UseItem(HammerID))
			CurrentState = NeedNails;
		break;
	case NeedHammer:
		if (Inventory->UseItem(HammerID))
			CurrentState = RepairedDoor;
		break;
	case NeedNails:
		if (Inventory->UseItem(NailsID))
			CurrentState = RepairedDoor;
		break;
	case RepairedDoor:
		CurrentState = FinalState;
		break;
	case FinalState:
		break;
	}

	// Avanzo la missione

	switch (CurrentState)
	{
	case InitialState:
		QuestManager->AdvanceQuest(QuestName, FirstStep);
		break;
	case NeedHammerNails:
		QuestManager->AdvanceQuest(QuestName, NeedHammerNailsStep);
		break;
	case NeedNails:
		QuestManager->AdvanceQuest(QuestName, NeedOneMoreItemStep);
		break;
	case NeedHammer:
		QuestManager->AdvanceQuest(QuestName, NeedOneMoreItemStep);
		break;
	case RepairedDoor:
		QuestManager->AdvanceQuest(QuestName, RepairedDoorStep);
		break;
	case FinalState:

		this->OpenChoiceMenu(Choices);
		//QuestManager->AdvanceQuest(QuestName, FinalStep);
		//APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		//UKismetSystemLibrary::QuitGame(GetWorld(), PlayerController, EQuitPreference::Quit, true);
		break;
	}
}

FString AAshley_QuestBunker::GetInteractMessage() const
{
	switch (CurrentState)
	{
		case AAshley_QuestBunker::InitialState:
			return "Open the Shelter Door";
			break;
		case AAshley_QuestBunker::NeedHammerNails:
		case AAshley_QuestBunker::NeedHammer:
		case AAshley_QuestBunker::NeedNails:
			return "Fix the Shelter Door";
			break;
		case AAshley_QuestBunker::RepairedDoor:
		case AAshley_QuestBunker::FinalState:
		default:
			return "Get inside the Shelter";
			break;
	}
}