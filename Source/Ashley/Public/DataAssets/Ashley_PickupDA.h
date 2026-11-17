// Ashley

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Tags_Enums_Structs/Ashley_RenderPickUpStructs.h"
#include "Tags_Enums_Structs/Ashley_MenuPickusStructs.h"

#include "Ashley_PickupDA.generated.h"

USTRUCT(BlueprintType)
struct FPickupActorConfigStruct
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FName ItemID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FInventoryInfo PickupItemsInventory;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FRenderPickup PickupItemsRender;
};

UCLASS(BlueprintType)
class ASHLEY_API UAshley_PickupDA : public UDataAsset
{
	GENERATED_BODY()
	
public:

   // virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (TitleProperty = ItemID))
    TArray<FPickupActorConfigStruct> PickupItemsData;

    // TODO: creare map (BeginPlay) per indicizzare tramite ItemID

    UFUNCTION(BlueprintCallable, Category = "PickUp")
    FRenderPickup GetRenderStructByID(FName ID);

    UFUNCTION(BlueprintCallable, Category = "PickUp")
    FInventoryInfo GetInventoryStructByID(FName ID);

    UFUNCTION(BlueprintCallable, Category = "PickUp")
    TArray<FName> GetIDsValids() const;
};
