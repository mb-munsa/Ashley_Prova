// Ashley


#include "Episode02/Ashley_QuestCar.h"

#include "Characters/Players/Ashley_CharacterPlayer.h"
#include "Components/Ashley_InventoryComponent.h"
#include "Managers/Ashley_QuestManager.h"
#include <Kismet/GameplayStatics.h>

void AAshley_QuestCar::BeginPlay()
{
	Super::BeginPlay();
}

void AAshley_QuestCar::Interact_Implementation()
{
	Super::Interact_Implementation();

	if (Character == nullptr)
		Character = Cast<AAshley_CharacterPlayer>(GetWorld()->GetFirstPlayerController()->GetCharacter());
	
	auto Inventory = Character->GetInventory();
	auto QuestManager = Character->GetQuestManager();

	switch (CurrentState)
	{
		case InitialState:
			if (Inventory->UseItem(CarKeyID))
				CurrentState = NeedPetrol;
			break;
		case NeedPetrol:
			if (Inventory->UseItem(PetrolTankID))
				CurrentState = GettingReady;
			break;
		case GettingReady:
			CurrentState = FinalState;
			break;
		case FinalState:
			break;
	}

	// Avanzo la missione

	switch (CurrentState)
	{
	case InitialState:
		QuestManager->AdvanceQuest(QuestName, NeedKeyDialogue);
		break;
	case NeedPetrol:
		QuestManager->AdvanceQuest(QuestName, NeedPetrolDialogue);
		break;
	case GettingReady:
		QuestManager->AdvanceQuest(QuestName, FinalDialogue);
		break;
	case FinalState:

		this->OpenChoiceMenu(Choices);

		//APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		///UKismetSystemLibrary::QuitGame(GetWorld(), PlayerController, EQuitPreference::Quit, true);
		break;
	}
}

FString AAshley_QuestCar::GetInteractMessage() const
{
	switch (CurrentState)
	{
	case InitialState:
		return "Open the car";
		break;
	case NeedPetrol:
		return "Refuel the tank";
		break;
	case FinalState:
	default:
		return "Drive away";
		break;
	}
}
