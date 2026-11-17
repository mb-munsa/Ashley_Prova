// Ashley


#include "DataAssets/Ashley_PickupDialogueDA.h"
#include "DataAssets/Ashley_PickupDA.h"

#include "Components/Ashley_InventoryComponent.h"

FText UAshley_PickupDialogueDA::GetDialogueForPickup(FName ItemID, UAshley_InventoryComponent* Inventory)
{
	// Controllo se esiste questo oggetto nella mappa
	FPickupDialogues Dialogue;
	bool Found = false;

	for (auto& DialogueForItem : Dialogues)
	{
		if (DialogueForItem.Key == ItemID)
		{
			Dialogue = DialogueForItem.Value;
			Found = true;
			break;
		}
	}

	if (!Found)
		return FText::GetEmpty();

	bool IsValid = true;

	for (auto& DialogueOption : Dialogue.DialogueChoices)
	{
		IsValid = true;
		for (auto& ItemRequired : DialogueOption.ItemsRequired)
		{
			// Controllo se il giocatore ha tutte gli item necessari
			//  alla prima opzione accettata, ritorno il dialogo
			if (!Inventory->IsItemPresent(ItemRequired))
				IsValid = false;
		}
		if (IsValid) return DialogueOption.Dialogue;
	}

	return FText::GetEmpty();
}

FText UAshley_PickupDialogueDA::GetGenericDialogueForPickup(FName ItemID)
{
	// Controllo se esiste questo oggetto nella mappa
	FPickupDialogues Dialogue;
	bool Found = false;

	for (auto& DialogueForItem : Dialogues)
	{
		if (DialogueForItem.Key == ItemID)
		{
			Dialogue = DialogueForItem.Value;
			Found = true;
			break;
		}
	}

	if (!Found)
		return FText::GetEmpty();

	return Dialogue.GenericDialogue;
}

TArray<FName> UAshley_PickupDialogueDA::GetItemNames() const
{
	TArray<FName> ItemsNames;

	if (ItemData)
	{
		for (const auto& PickUp : ItemData->PickupItemsData)
		{
			ItemsNames.Add(PickUp.ItemID);
		}
	}

	return ItemsNames;
}
