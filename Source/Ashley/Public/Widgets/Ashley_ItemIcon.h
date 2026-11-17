// Ashley

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Ashley_ItemIcon.generated.h"

class UTexture2D;
class UTextBlock;
class UImage;

class UAshley_InventoryUI;
class UAshley_PickupDA;

/**
 * 
 */
UCLASS()
class ASHLEY_API UAshley_ItemIcon : public UUserWidget
{
	GENERATED_BODY()

public:

	void NativePreConstruct() override;

	UFUNCTION()
	void SetItemIcon(UTexture2D* Texture, const FName& MyItemID);
	UFUNCTION()
	void SetStackNumber(int StackNumber);
	UFUNCTION()
	void SetIconVisibility(ESlateVisibility NewVisibility);
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void SelectItem(const FName& SelectedItemID);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void ZoomInItem(const FName& SelectedItemID);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void ZoomOutItem(const FName& SelectedItemID);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void ShowItemInfo(const FName& SelectedItemID);

	UFUNCTION()
	void SetInventoryRef(UAshley_InventoryUI* InventoryController);

	UFUNCTION(BlueprintCallable)
	FName GetItemID() const;

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;

	UFUNCTION()
	TArray<FName> GetValidIDs() const;
#endif

protected:

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UImage* Icon;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* StackNumberText;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Data|Inventory")
	UTexture2D* ItemTexture;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Data|Inventory", meta = (GetOptions = "GetValidIDs"))
	FName ItemID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data|Inventory")
	TSoftObjectPtr<UAshley_PickupDA> PickupDataAsset;

	UPROPERTY(BlueprintReadWrite)
	UAshley_InventoryUI* InventoryUI;
};
