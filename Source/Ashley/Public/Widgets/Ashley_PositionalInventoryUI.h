// Ashley

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Tags_Enums_Structs/Ashley_MenuPickusStructs.h"
#include "Widgets/Ashley_InventoryUI.h"

#include "Ashley_PositionalInventoryUI.generated.h"

class UTextBlock;
class UCanvasPanel;
class UAshley_PickupDA;
class UAshley_ItemIcon;
class UAshley_InventoryUI;

/**
 * 
 */
UCLASS()
class ASHLEY_API UAshley_PositionalInventoryUI : public UAshley_InventoryUI
{
	GENERATED_BODY()

public:

	virtual void NativeConstruct() override;

	void UpdateItemsInfo(const TArray<FItemStack>& items) override;

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<UAshley_ItemIcon*> ItemSlots;
};
