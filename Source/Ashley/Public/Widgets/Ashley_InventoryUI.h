// Ashley

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Tags_Enums_Structs/Ashley_MenuPickusStructs.h"
#include "Ashley_InventoryUI.generated.h"

class UAshley_PickupDA;
class UAshley_ItemIcon;
class UAshley_QuestWidget;
class UPanelWidget;
class UVerticalBox;
class UImage;
class UTextBlock;
struct FActiveQuest;

/**
 * 
 */
UCLASS()
class ASHLEY_API UAshley_InventoryUI : public UUserWidget
{
	GENERATED_BODY()

public:

	virtual void NativeConstruct() override;

	UFUNCTION()
	virtual void UpdateItemsInfo(const TArray<FItemStack>& items);

	UFUNCTION()
	virtual void UpdateQuestsInfo(const TArray<FActiveQuest>& quests);

	UFUNCTION()
	virtual void ShowItemInfo(const FName& ItemName);

	UFUNCTION()
	virtual void SelectItem(const FName& ItemName);

	UFUNCTION(BlueprintImplementableEvent)
	void ZoomInItem(const FName& ItemName);

	UFUNCTION(BlueprintImplementableEvent)
	void ZoomOutItem(const FName& ItemName);

	UFUNCTION(BlueprintImplementableEvent)
	void OnOpen();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UAshley_ItemIcon> BP_ItemIcon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UAshley_QuestWidget> BP_QuestWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<UAshley_PickupDA*> ItemsDataAsset;

	UPROPERTY(meta = (BindWidget))
	UPanelWidget* InventoryContainer;

	//UPROPERTY(meta = (BindWidget))
	//UVerticalBox* QuestsContainer;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* ItemNameText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* ItemDescriptionText;

	UPROPERTY()
	TMap<FName, FInventoryInfo> CurrentItems;
};
