// Ashley

#pragma once

#include "CoreMinimal.h"
#include "Interaction/Ashley_ActorInteract.h"

#include "Ashley_Pickup.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnItemPickup, FName);
//DECLARE_MULTICAST_DELEGATE(FOnHouseDoor);

class UAshley_PickupDA;
class UAshley_QuestDA;
class UAshley_InventoryComponent;
class AAshley_FixedCameraController;

UCLASS()
class ASHLEY_API AAshley_Pickup : public AAshley_ActorInteract
{
	GENERATED_BODY()

public:
	AAshley_Pickup();

    virtual void Interact_Implementation() override;

#if WITH_EDITOR
    virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data|Config", meta = (GetOptions = "GetValidIDs"))
    FName Name_ID;

    UFUNCTION()
    TArray<FName> GetValidIDs() const;

    UFUNCTION()
    void AddToInventory(UAshley_InventoryComponent* Inventory) const;

    static FOnItemPickup OnItemPickup;

protected:

    virtual void BeginPlay() override;

   UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data|Inventory")
   TSoftObjectPtr<UAshley_PickupDA> PickupDataAsset;

   UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data|Various")
   AAshley_FixedCameraController* ZoomInCamera;

   UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data|Mesh")
   TObjectPtr<UStaticMeshComponent> MeshBase;

  // virtual void OnBoxInteractOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
  // virtual void OnBoxInteractEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;
   
   //UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data|Quest")
   //TSoftObjectPtr<UAshley_QuestDA> QuestDataAsset;

   //UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data|Quest")
   //bool bShouldUpdateMission;

   //UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data|Quest", meta = (GetOptions = "GetQuestNames"))
   //FName QuestName;

   //UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data|Quest")
   //int QuestStep;

   UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data|Inventory")
   int StackNumber = 1;

   //UFUNCTION()
   //TArray<FName> GetQuestNames() const;
private:

    UFUNCTION()
    void UpdateMesh(FRenderPickup RenderStruct);

    UFUNCTION()
    void UpdateMaterialsBySlot(FRenderPickup RenderStruct);

    //UPROPERTY()
    //bool bIsCollectable;

    UPROPERTY()
    FInventoryInfo InventoryInfo;
    //int32 NumberKeyPuzzle;

};
