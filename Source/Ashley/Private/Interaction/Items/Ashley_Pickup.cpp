// Ashley


#include "Interaction/Items/Ashley_Pickup.h"
#include "Engine/StaticMesh.h"
//#include "Tags_Enums_Structs/Ashley_MenuPickusStructs.h"

#include "DataAssets/Ashley_PickupDA.h"
#include "DataAssets/Ashley_QuestDA.h"

// TODO: migliorare
#include "Characters/Players/Ashley_CharacterPlayer.h"
#include "Managers/Ashley_QuestManager.h"
#include "Components/Ashley_InventoryComponent.h"
#include "Core/Ashley_FixedCameraController.h"
#include "Core/Ashley_PlayerController.h"
#include "Components/Ashley_CameraManager.h"


#include "DebugHelper.h"

FOnItemPickup AAshley_Pickup::OnItemPickup;

AAshley_Pickup::AAshley_Pickup()
{
    PrimaryActorTick.bCanEverTick = false;

    MeshBase = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshBase"));
    MeshBase->SetupAttachment(RootComponent);

    // Configuro il layer per essere interagibile dal player
    MeshBase->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Block);
    MeshBase->SetRenderCustomDepth(true);
    MeshBase->SetCustomDepthStencilValue(0);
}

void AAshley_Pickup::BeginPlay()
{
	Super::BeginPlay();

    UAshley_PickupDA* LoadedAsset = PickupDataAsset.LoadSynchronous();

    if (!LoadedAsset)
    {
        UE_LOG(LogTemp, Warning, TEXT("PickupDataAsset is not assigned in %s"), *GetName());
        return;
    }

    //FInventoryInfo
    InventoryInfo = LoadedAsset->GetInventoryStructByID(Name_ID);

    /////////////

    //if (InventoryInfo.ItemMainType == EPickupType::COLLECTABLE) bIsCollectable = true;
    //else bIsCollectable = false;

    //if (InventoryInfo.KeyNumber != NULL) NumberKeyPuzzle = InventoryInfo.KeyNumber;
    //else NumberKeyPuzzle = 0;
}

//void AAshley_Pickup::OnBoxInteractOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
//{
//    Super::OnBoxInteractOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
//
//    if (CharacterInRange)
//    {
//        MeshBase->SetCustomDepthStencilValue(1);
//    }
//
//}

//void AAshley_Pickup::OnBoxInteractEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
//{
//    Super::OnBoxInteractEndOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
//    MeshBase->SetCustomDepthStencilValue(0);
//}

#if WITH_EDITOR
void AAshley_Pickup::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
    Super::PostEditChangeProperty(PropertyChangedEvent);
     FName PropertyName = (PropertyChangedEvent.Property != nullptr)
        ? PropertyChangedEvent.Property->GetFName()
        : NAME_None;

    if (PropertyName == GET_MEMBER_NAME_CHECKED(AAshley_Pickup, Name_ID))
    {
        UAshley_PickupDA* LoadedAsset;
        if (PickupDataAsset.IsNull())
        {
            UE_LOG(LogTemp, Warning, TEXT("PickupDataAsset is not assigned in %s"), *GetName());
            return;
        }

        if (PickupDataAsset.IsValid()) LoadedAsset = PickupDataAsset.Get();
        else LoadedAsset = PickupDataAsset.LoadSynchronous();

        if (LoadedAsset)
        {
            FRenderPickup RenderStruct = LoadedAsset->GetRenderStructByID(Name_ID);
            UpdateMesh(RenderStruct);
            if (RenderStruct.bNeedsMaterialAssigned) UpdateMaterialsBySlot(RenderStruct);
        }
    }
}
#endif

TArray<FName> AAshley_Pickup::GetValidIDs() const
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

void AAshley_Pickup::AddToInventory(UAshley_InventoryComponent* Inventory) const
{
    Inventory->PutObjectInventoryWithInfo(Name_ID, InventoryInfo, StackNumber == 0 ? 1 : StackNumber);
}

void AAshley_Pickup::UpdateMesh(FRenderPickup RenderStruct)
{
	if (!MeshBase)
	{
		UE_LOG(LogTemp, Error, TEXT("MeshBase is not assigned in %s"), *GetName());
		return;
	}

   UStaticMesh* Mesh = RenderStruct.Mesh;

    if (Mesh)
    {
        MeshBase->SetStaticMesh(Mesh);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Can't access to StaticMesh of %s"), *GetName());
    }
}

void AAshley_Pickup::UpdateMaterialsBySlot(FRenderPickup RenderStruct)
{
    if (!MeshBase || !MeshBase->GetStaticMesh() || !PickupDataAsset.IsValid()) return;

    TArray<FMaterialBySlot> MaterialBySlot = RenderStruct.MaterialsBySlot;

    int32 i = 0;
    UStaticMesh* Mesh = MeshBase->GetStaticMesh();
    int32 NumMaterialSlots = Mesh->GetStaticMaterials().Num();

    for (const auto& Elem : MaterialBySlot)
    {
        UMaterialInterface* Material = Elem.Material;

        //FName SlotName = PickupDataAsset->GetSlotNameFromLabel(Elem.SlotLabel);

        if (i >= NumMaterialSlots)
        {
            UE_LOG(LogTemp, Warning, TEXT("Too materials on DataAsset for the mesh %s"), *GetName());
            return;
        }

        MeshBase->SetMaterial(i, Material);
        i++;
      /* if (SlotName != NAME_None && MeshBase->GetMaterialIndex(SlotName) != INDEX_NONE)
        {
            MeshBase->SetMaterialByName(SlotName, Material);
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("Slot '%s' is not on the mesh %s"), *SlotName.ToString(), *GetName());
        }*/
    }
}

//TArray<FName> AAshley_Pickup::GetQuestNames() const
//{
//    TArray<FName> QuestNames;
//
//    UAshley_QuestDA* LoadedAsset;
//    if (QuestDataAsset.IsValid())
//        LoadedAsset = QuestDataAsset.Get();
//    else
//        LoadedAsset = QuestDataAsset.LoadSynchronous();
//
//    if (LoadedAsset)
//    {
//        for (const auto& Quest : LoadedAsset->Quests)
//        {
//            QuestNames.Add(Quest.QuestID);
//        }
//    }
//
//    return QuestNames;
//}

void AAshley_Pickup::Interact_Implementation()
{
    auto Character = Cast<AAshley_CharacterPlayer>(GetWorld()->GetFirstPlayerController()->GetCharacter());

    if (Character)
    {
        // Se era presente una camera apposita per zoomIn sull'oggetto,
        //  la distruggo e setto come attiva la camera precedente
        if (ZoomInCamera)
        {
            UAshley_CameraManager::Get(this)->RemoveFromCameraStack(ZoomInCamera);

            ZoomInCamera->Destroy();
        }

        Character->PickupItem(Name_ID, InventoryInfo, StackNumber);

        this->Destroy();

        // NOTA: al momento delego al player il broadcast dell'evento
        //AAshley_Pickup::OnItemPickup.Broadcast(Name_ID);
    }
}


