// Ashley

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Tags_Enums_Structs/Ashley_MenuPickusStructs.h"

#include "Ashley_InventoryComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ASHLEY_API UAshley_InventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UAshley_InventoryComponent();

	UFUNCTION(BlueprintCallable)
	bool PutObjectInventoryWithInfo(FName ID_Name, FInventoryInfo InventoryInfo, int StackSize = 1);

	UFUNCTION(BlueprintCallable)
	bool PutObjectInventory(FName ID_Name, EPickupType ItemMainType = EPickupType::OBJECT, int StackSize = 1);

	// Ripristina un collectable portato dal livello precedente, senza mostrare la notifica
	UFUNCTION()
	void RestoreCollectable(FName ID_Name);

	UFUNCTION(BlueprintCallable)
	void AddCarriedItem(FName ID_Name);

	UFUNCTION(BlueprintCallable)
	void RemoveCarriedItem();

	UFUNCTION(BlueprintCallable)
	void DeleteObjectInventory(FName ID_Name);

	UFUNCTION(BlueprintCallable)
	bool UseItem(FName ItemName);

	UFUNCTION(BlueprintCallable)
	bool IsItemPresent(FName ItemName);

	UFUNCTION(BlueprintCallable)
	int GetItemCount(FName ID_Name);

	UFUNCTION()
	TArray<FItemStack> GetAllItems();

	UFUNCTION()
	TArray<FItemStack> GetCollectables();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data|Inventory")
	int32 SlotNumber;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	UPROPERTY()
	TMap<FName, int> InventoryObjectsID;

	UPROPERTY()
	TMap<FName, bool> InventoryCollectables;

	UPROPERTY()
	FName CarriedItem;

};
