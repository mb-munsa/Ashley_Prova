// Ashley


#include "DataAssets/Ashley_PickupDA.h"

FRenderPickup UAshley_PickupDA::GetRenderStructByID(FName ID)
{
	for (const FPickupActorConfigStruct& Pickup : PickupItemsData)
	{
		if (Pickup.ItemID == ID)
		{
			return Pickup.PickupItemsRender;
		}
	}
	return FRenderPickup();

}

FInventoryInfo UAshley_PickupDA::GetInventoryStructByID(FName ID)
{
	for (const FPickupActorConfigStruct& Config : PickupItemsData)
	{
		if (Config.ItemID == ID)
		{
			return Config.PickupItemsInventory;
		}
	}
	return FInventoryInfo();
}

TArray<FName> UAshley_PickupDA::GetIDsValids() const
{
	TArray<FName> ID;

	for (const FPickupActorConfigStruct& Pickup : PickupItemsData)
	{
		ID.Add(Pickup.ItemID);
	}
	return ID;
}
