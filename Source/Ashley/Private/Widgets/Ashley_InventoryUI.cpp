// Ashley

#include "Widgets/Ashley_InventoryUI.h"
#include "Widgets/Ashley_ItemIcon.h"
#include "Widgets/Ashley_QuestWidget.h"
#include "Managers/Ashley_EventManagerSystem.h"
#include "Managers/Ashley_QuestManager.h"
#include "DataAssets/Ashley_PickupDA.h"

#include "Components/ActorComponent.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/VerticalBox.h"
#include "Components/PanelWidget.h"
#include "Components/HorizontalBoxSlot.h"
#include "Engine/Texture2D.h"
#include "Blueprint/WidgetTree.h"

#include "DebugHelper.h"
#include <Kismet/GameplayStatics.h>

void UAshley_InventoryUI::NativeConstruct()
{
	Super::NativeConstruct();

	for (int i = 0; i < InventoryContainer->GetChildrenCount(); i++)
	{
		auto NewIcon = Cast<UAshley_ItemIcon>(InventoryContainer->GetChildAt(i));
		NewIcon->SetInventoryRef(this);
	}
}

void UAshley_InventoryUI::UpdateItemsInfo(const TArray<FItemStack>& items)
{
	if (!InventoryContainer) return;

	CurrentItems.Reset();

	int idx = 0;

	for (const auto& item : items)
	{
		FPickupActorConfigStruct* itemConfig = nullptr;

		// Cerco l'item in ogni dataAsset pickups
		for (auto data : ItemsDataAsset)
		{
			itemConfig = data->PickupItemsData.FindByPredicate(
				[item](FPickupActorConfigStruct PickUp)
				{ return PickUp.ItemID == item.ItemID; }
			);

			if (itemConfig != nullptr) break;
		}

		if (itemConfig == nullptr)
		{
			Debug::Print("Item non trovato!");
			return;
		}

		FInventoryInfo itemInvInfo = itemConfig->PickupItemsInventory;
		auto icon = itemInvInfo.Icon;

		auto NewIcon = Cast<UAshley_ItemIcon>(InventoryContainer->GetChildAt(idx));
		NewIcon->SetItemIcon(icon, itemConfig->ItemID);
		NewIcon->SetStackNumber(item.StackSize);
		NewIcon->SetIconVisibility(ESlateVisibility::Visible);

		idx++;
		CurrentItems.Add(itemConfig->ItemID, itemInvInfo);
	}

	// TODO: cache in a TArray the icons
	for (idx; idx < InventoryContainer->GetChildrenCount(); idx++)
	{
		auto NewIcon = Cast<UAshley_ItemIcon>(InventoryContainer->GetChildAt(idx));
		NewIcon->SetItemIcon(nullptr, NAME_None);
		NewIcon->SetStackNumber(0);
		NewIcon->SetIconVisibility(ESlateVisibility::Hidden);
	}

	ItemNameText->SetText(FText::GetEmpty());
	ItemDescriptionText->SetText(FText::GetEmpty());

	OnOpen();
}

void UAshley_InventoryUI::UpdateQuestsInfo(const TArray<FActiveQuest>& quests)
{
	//if (!QuestsContainer) return;

	//QuestsContainer->ClearChildren();

	//if (quests.IsEmpty()) return;

	//for (const auto& quest : quests)
	//{
	//	//Debug::Print(FString::Printf(TEXT("Found quest name %s"), *(quest.QuestName.ToString())));
	//	auto NewQuest = WidgetTree->ConstructWidget<UAshley_QuestWidget>(BP_QuestWidget);
	//	NewQuest->SetQuestName(quest.QuestName.ToString());
	//	NewQuest->SetQuestText(quest.CurrentQuestDesc);

	//	QuestsContainer->AddChild(NewQuest);
	//}
}

void UAshley_InventoryUI::ShowItemInfo(const FName& ItemName)
{
	auto item = CurrentItems.Find(ItemName);

	if (item)
	{
		ItemNameText->SetText(item->ItemName);
		ItemDescriptionText->SetText(item->ItemDescription);
	}
	else
	{
		ItemNameText->SetText(FText::GetEmpty());
		ItemDescriptionText->SetText(FText::GetEmpty());
	}
}

void UAshley_InventoryUI::SelectItem(const FName& ItemName)
{
	//auto item = CurrentItems.Find(ItemName);
	if (ItemName.IsNone()) return;

	auto EventManager = UGameplayStatics::GetGameInstance(this)->GetSubsystem<UAshley_EventManagerSystem>();
	EventManager->BroadcastItemSelected(ItemName);
}
