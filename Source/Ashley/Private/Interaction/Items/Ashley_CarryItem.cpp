// Ashley


#include "Interaction/Items/Ashley_CarryItem.h"
#include "Interaction/Items/Ashley_Pickup.h"
#include "Characters/Players/Ashley_CharacterPlayer.h"
#include "DataAssets/Ashley_PickupDA.h"
#include "Components/BoxComponent.h"

#include "DebugHelper.h"


AAshley_CarryItem::AAshley_CarryItem()
{
	PrimaryActorTick.bCanEverTick = false;

	// Setto la mesh come root, per poter abilitare la fisica
	MeshBase = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshBase"));
	SetRootComponent(MeshBase);
	BoxInteract->SetupAttachment(MeshBase);
	MeshBase->SetSimulatePhysics(true);

	MeshBase->SetRenderCustomDepth(true);
	MeshBase->SetCustomDepthStencilValue(1);

	// Configuro il layer per essere interagibile dal player
	MeshBase->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Block);
}

void AAshley_CarryItem::Interact_Implementation()
{
	if (!CharacterRef) CharacterRef = Cast<AAshley_CharacterPlayer>(GetWorld()->GetFirstPlayerController()->GetCharacter());

	// Disattivo tutte le collisioni e la fisica
	bIgnoreCollision = true;
	MeshBase->SetSimulatePhysics(false);
	PreviousCollisionPreset = MeshBase->GetCollisionEnabled();
	MeshBase->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	MeshBase->SetCustomDepthStencilValue(0);

	// lo notifico al player per attacco al socket
	CharacterRef->CarryItem(this);

	bIsBeingCarried = true;
	AAshley_Pickup::OnItemPickup.Broadcast(Name_ID);
}

void AAshley_CarryItem::Drop()
{
	this->SetActorRotation(FQuat::Identity);

	// Riabilito collisioni e fisica
	bIgnoreCollision = false;
	MeshBase->SetCollisionEnabled(PreviousCollisionPreset);
	MeshBase->SetSimulatePhysics(true);
	MeshBase->SetCustomDepthStencilValue(1);

	bIsBeingCarried = false;
}

FName AAshley_CarryItem::GetID()
{
	return Name_ID;
}

bool AAshley_CarryItem::IsDestroyOnUse()
{
	return bIsDestroyOnUse;
}

// UTILITY
TArray<FName> AAshley_CarryItem::GetValidIDs() const
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
