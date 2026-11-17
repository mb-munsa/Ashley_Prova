// Ashley

#include "Widgets/Ashley_PositionalInventoryUI.h"
#include "Widgets/Ashley_ItemIcon.h"

#include "Components/PanelWidget.h"
#include "Components/TextBlock.h"

#include "Managers/Ashley_EventManagerSystem.h"
#include "DataAssets/Ashley_PickupDA.h"

void UAshley_PositionalInventoryUI::NativeConstruct()
{
	Super::NativeConstruct();

	// Prendo tutte le referenze delle icone
	for (int i = 0; i < InventoryContainer->GetChildrenCount(); i++)
	{
		auto NewIcon = Cast<UAshley_ItemIcon>(InventoryContainer->GetChildAt(i));
		ItemSlots.Add(NewIcon);
	}
}

void UAshley_PositionalInventoryUI::UpdateItemsInfo(const TArray<FItemStack>& items)
{
	CurrentItems.Reset();

	// scorro gli slot, e li setto attivi se l'oggetto è presente
	for (const auto& ItemSlot : ItemSlots)
	{
		// Se l'item presente in questo slot è presente tra gli items passati, lo attivo
		if (items.ContainsByPredicate(
			[ItemSlot](FItemStack ItemStack)
			{ return ItemStack.ItemID == ItemSlot->GetItemID(); }
		))
		{
			FPickupActorConfigStruct* itemConfig = nullptr;

			for (auto data : ItemsDataAsset)
			{
				itemConfig = data->PickupItemsData.FindByPredicate(
					[ItemSlot](FPickupActorConfigStruct PickUp)
					{ return PickUp.ItemID == ItemSlot->GetItemID(); });

				if (itemConfig != nullptr) break;
			}

			if (itemConfig == nullptr) return;

			FInventoryInfo itemInvInfo = itemConfig->PickupItemsInventory;
			CurrentItems.Add(itemConfig->ItemID, itemInvInfo);

			ItemSlot->SetIconVisibility(ESlateVisibility::Visible);
		}
		else
		{
			ItemSlot->SetIconVisibility(ESlateVisibility::Collapsed);
		}
	}

	ItemNameText->SetText(FText::GetEmpty());
	ItemDescriptionText->SetText(FText::GetEmpty());

	OnOpen();
}

