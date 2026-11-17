// Ashley


#include "Widgets/Ashley_CollectableNotification.h"
#include "DataAssets/Ashley_PickupDA.h"

#include "Animation/WidgetAnimation.h"
#include "Components/Image.h"

bool UAshley_CollectableNotification::SetItem(FName ItemID)
{
	// Cerco l'item in ogni dataAsset pickups
	for (auto data : ItemsDataAsset)
	{
		if (!data) continue;

		const FPickupActorConfigStruct* itemConfig = data->PickupItemsData.FindByPredicate(
			[ItemID](const FPickupActorConfigStruct& PickUp)
			{ return PickUp.ItemID == ItemID; }
		);

		if (itemConfig != nullptr && itemConfig->PickupItemsInventory.Icon != nullptr)
		{
			Icon->SetBrushFromTexture(itemConfig->PickupItemsInventory.Icon);
			return true;
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("%s: nessuna icona trovata per il collectable %s"), *GetName(), *ItemID.ToString());
	return false;
}

void UAshley_CollectableNotification::PlayShowAnimation()
{
	PlayAnimationForward(ShowAnimation);
}

float UAshley_CollectableNotification::PlayHideAnimation()
{
	PlayAnimationReverse(ShowAnimation);

	return ShowAnimation->GetEndTime();
}
