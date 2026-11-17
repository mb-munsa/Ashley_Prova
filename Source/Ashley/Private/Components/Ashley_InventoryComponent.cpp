// Ashley


#include "Components/Ashley_InventoryComponent.h"
#include "Characters/Players/Ashley_CharacterPlayer.h"
#include "Managers/Ashley_LevelTransitionSubsystem.h"
#include "Managers/Ashley_WidgetSystem.h"

#include "DebugHelper.h"

UAshley_InventoryComponent::UAshley_InventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	SlotNumber = 16;
}

void UAshley_InventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	// Ripristino gli item portati dal livello precedente
	GetWorld()->GetGameInstance()->GetSubsystem<UAshley_LevelTransitionSubsystem>()->RestoreInventory(this);
}

bool UAshley_InventoryComponent::PutObjectInventoryWithInfo(FName ID_Name, FInventoryInfo InventoryInfo, int StackSize)
{
	return PutObjectInventory(ID_Name, InventoryInfo.ItemMainType, StackSize);
}

bool UAshley_InventoryComponent::PutObjectInventory(FName ID_Name, EPickupType ItemMainType, int StackSize)
{
	// Collectables
	if (ItemMainType == EPickupType::COLLECTABLE)
	{
		if (!InventoryCollectables.Contains(ID_Name))
		{
			InventoryCollectables.Add(ID_Name, true);
			GetWorld()->GetGameInstance()->GetSubsystem<UAshley_WidgetSystem>()->ShowCollectableNotification(ID_Name);
		}

		return true;
	}

	// Cerco se è già presente questo item
	auto ItemPresent = InventoryObjectsID.Contains(ID_Name);

	if (ItemPresent)
	{
		InventoryObjectsID[ID_Name] += StackSize;
		return true;
	}
	else if (InventoryObjectsID.Num() < SlotNumber)
	{
		InventoryObjectsID.Add(ID_Name, StackSize);
		return true;
	}

	return false;
}

void UAshley_InventoryComponent::RestoreCollectable(FName ID_Name)
{
	InventoryCollectables.Add(ID_Name, true);
}

void UAshley_InventoryComponent::AddCarriedItem(FName ID_Name)
{
	CarriedItem = ID_Name;
}

void UAshley_InventoryComponent::RemoveCarriedItem()
{
	CarriedItem = NAME_None;
}

void UAshley_InventoryComponent::DeleteObjectInventory(FName ID_Name)
{
	InventoryObjectsID[ID_Name] -= 1;

	if (InventoryObjectsID[ID_Name] <= 0)
	{
		InventoryObjectsID.Remove(ID_Name);
	}
}

bool UAshley_InventoryComponent::UseItem(FName ItemName)
{
	if (InventoryObjectsID.Contains(ItemName))
	{
		// TODO: usare item, se è consumabile
		//DeleteObjectInventory(ItemName);
		return true;
	}

	if (ItemName == CarriedItem)
	{
		auto Character = Cast<AAshley_CharacterPlayer>(GetOwner());
		Character->UseCarriedItem();
		return true;
	}

	return false;
}

bool UAshley_InventoryComponent::IsItemPresent(FName ItemName)
{
	if (InventoryObjectsID.Contains(ItemName))
		return true;

	if (InventoryCollectables.Contains(ItemName))
		return true;

	if (ItemName == CarriedItem)
		return true;

	return false;
}

int UAshley_InventoryComponent::GetItemCount(FName ID_Name)
{
	if (InventoryObjectsID.Contains(ID_Name)) return InventoryObjectsID[ID_Name];
	else return 0;
}

TArray<FItemStack> UAshley_InventoryComponent::GetAllItems()
{
	TArray<FItemStack> result;

	for (const auto& Item : InventoryObjectsID)
	{
		result.Add(FItemStack(Item.Value, Item.Key));
	}

	return result;
}

TArray<FItemStack> UAshley_InventoryComponent::GetCollectables()
{
	TArray<FItemStack> result;

	for (const auto& Item : InventoryCollectables)
	{
		result.Add(FItemStack(1, Item.Key));
	}

	return result;
}



