// Ashley


#include "Interaction/Items/Ashley_ItemShowcase.h"
#include "Components/WidgetComponent.h"
#include "DataAssets/Ashley_PickupDA.h"
#include "CineCameraComponent.h"

// Sets default values
AAshley_ItemShowcase::AAshley_ItemShowcase()
{
    CameraOverlay = CreateDefaultSubobject<UWidgetComponent>(TEXT("CameraOverlay"));
    CameraOverlay->SetupAttachment(RootComponent);

    Camera = CreateDefaultSubobject<UCineCameraComponent>(TEXT("Camera"));
    Camera->SetupAttachment(CameraOverlay);
}

// Called when the game starts or when spawned
void AAshley_ItemShowcase::BeginPlay()
{
    Super::BeginPlay();

    ItemMap.Reset();

    TArray<UStaticMeshComponent*> AllMeshes;
    GetComponents<UStaticMeshComponent>(AllMeshes);

    UAshley_PickupDA* LoadedAsset;
    if (PickupDataAsset.IsNull())
    {
        UE_LOG(LogTemp, Warning, TEXT("PickupDataAsset is not assigned in %s"), *GetName());
        return;
    }

    if (PickupDataAsset.IsValid()) LoadedAsset = PickupDataAsset.Get();
    else LoadedAsset = PickupDataAsset.LoadSynchronous();

    for (UStaticMeshComponent* Mesh : AllMeshes)
    {
        for (auto& Name : LoadedAsset->GetIDsValids())
        {
            if (Mesh->GetName() == Name)
            {
                FItemMeshPair& Pair = ItemMap.Add_GetRef(FItemMeshPair(Name, Mesh));
                Pair.InitialRelativeTransform = Mesh->GetRelativeTransform();
            }
        }
    }
}

void AAshley_ItemShowcase::ShowItem(FName ItemName)
{
    CurrentMesh = nullptr;

    for (auto& Pair : ItemMap)
    {
        if (Pair.ItemName == ItemName)
        {
            // Ogni oggetto riparte dalla sua orientazione di default
            Pair.MeshComponent->SetRelativeTransform(Pair.InitialRelativeTransform);
            Pair.MeshComponent->SetVisibility(true);
            CurrentMesh = Pair.MeshComponent;
        }
        else Pair.MeshComponent->SetVisibility(false);
    }

    this->SetActorHiddenInGame(false);

    GetWorld()->GetFirstPlayerController()->SetViewTarget(this);

    return;
}

void AAshley_ItemShowcase::CloseShowcase()
{
    this->SetActorHiddenInGame(true);
}

void AAshley_ItemShowcase::SetOverscan(float OverscanAmount)
{
    Camera->Overscan = OverscanAmount;
}

void AAshley_ItemShowcase::RotateItem(FVector2D Delta)
{
    if (!IsValid(CurrentMesh) || Delta.IsNearlyZero()) return;

    const float PitchSign = bInvertPitch ? -1.f : 1.f;

    // Assi della camera: il movimento risulta intuitivo indipendentemente dalla rotazione attuale dell'oggetto
    const FQuat YawQuat(Camera->GetUpVector(), FMath::DegreesToRadians(-Delta.X * RotationSpeed));
    const FQuat PitchQuat(Camera->GetRightVector(), FMath::DegreesToRadians(Delta.Y * RotationSpeed * PitchSign));
    const FQuat DeltaQuat = YawQuat * PitchQuat;

    // Ruoto attorno al centro dei bounds e non al pivot della mesh, che potrebbe non essere centrato
    const FVector Pivot = CurrentMesh->Bounds.Origin;
    const FTransform CurrentTransform = CurrentMesh->GetComponentTransform();
    const FVector NewLocation = Pivot + DeltaQuat.RotateVector(CurrentTransform.GetLocation() - Pivot);

    CurrentMesh->SetWorldLocationAndRotation(NewLocation, DeltaQuat * CurrentTransform.GetRotation());
}


#if WITH_EDITOR

TArray<FName> AAshley_ItemShowcase::GetValidIDs() const
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

