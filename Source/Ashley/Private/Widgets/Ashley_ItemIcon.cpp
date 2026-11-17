// Ashley


#include "Widgets/Ashley_ItemIcon.h"

#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "DataAssets/Ashley_PickupDA.h"

#include "Widgets/Ashley_InventoryUI.h"

void UAshley_ItemIcon::NativePreConstruct()
{
    Super::NativePreConstruct();

    if (Icon)
        Icon->SetBrushFromTexture(ItemTexture);
}

void UAshley_ItemIcon::SetItemIcon(UTexture2D* Texture, const FName& MyItemID)
{
	Icon->SetBrushFromTexture(Texture);
	this->ItemID = MyItemID;
}

void UAshley_ItemIcon::SetStackNumber(int StackNumber)
{
	if (StackNumber < 2) StackNumberText->SetVisibility(ESlateVisibility::Collapsed);
	else
	{
		StackNumberText->SetText(FText::FromString(FString::FromInt(StackNumber)));
		StackNumberText->SetVisibility(ESlateVisibility::Visible);
	}
}

void UAshley_ItemIcon::SetIconVisibility(ESlateVisibility NewVisibility)
{
	Icon->SetVisibility(NewVisibility);
}

void UAshley_ItemIcon::SelectItem(const FName& SelectedItemID)
{
	InventoryUI->SelectItem(SelectedItemID);
}

void UAshley_ItemIcon::ZoomInItem(const FName& SelectedItemID)
{
    InventoryUI->ZoomInItem(SelectedItemID);
}

void UAshley_ItemIcon::ZoomOutItem(const FName& SelectedItemID)
{
    InventoryUI->ZoomOutItem(SelectedItemID);
}

void UAshley_ItemIcon::ShowItemInfo(const FName& SelectedItemID)
{
	InventoryUI->ShowItemInfo(SelectedItemID);
}

void UAshley_ItemIcon::SetInventoryRef(UAshley_InventoryUI* InventoryController)
{
	InventoryUI = InventoryController;
}

FName UAshley_ItemIcon::GetItemID() const
{
	return ItemID;
}


#if WITH_EDITOR
void UAshley_ItemIcon::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
    Super::PostEditChangeProperty(PropertyChangedEvent);
    FName PropertyName = (PropertyChangedEvent.Property != nullptr)
        ? PropertyChangedEvent.Property->GetFName()
        : NAME_None;

    if (PropertyName == GET_MEMBER_NAME_CHECKED(UAshley_ItemIcon, ItemTexture))
    {
        if (Icon != nullptr && ItemTexture != nullptr)
			Icon->SetBrushFromTexture(ItemTexture);
    }
}

TArray<FName> UAshley_ItemIcon::GetValidIDs() const
{
    UAshley_PickupDA* LoadedAsset;
    if (PickupDataAsset.IsValid())
    {
        LoadedAsset = PickupDataAsset.Get();
    }
    else
    {
        LoadedAsset = PickupDataAsset.LoadSynchronous();
    }
    TArray<FName> IDs;
    if (LoadedAsset)
    {
        IDs = LoadedAsset->GetIDsValids();
    }
    return IDs;
}

#endif