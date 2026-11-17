// Ashley


#include "Interaction/Ashley_MinivanPuzzle.h"
#include "DataAssets/Ashley_PickupDA.h"
#include <Characters/Players/Ashley_CharacterPlayer.h>
#include "Components/Ashley_InventoryComponent.h"


// Sets default values
AAshley_MinivanPuzzle::AAshley_MinivanPuzzle()
{
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);

	// TODO: aggiungere layer clickable
	// Configuro il layer per essere interagibile dal player
	//Mesh->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Block);
	//Mesh->SetRenderCustomDepth(true);
	//Mesh->SetCustomDepthStencilValue(0);
}

void AAshley_MinivanPuzzle::StartPuzzle_Implementation()
{
	Mesh->SetVisibility(false);
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AAshley_MinivanPuzzle::ExitPuzzle_Implementation()
{
	if (bIsPuzzleCompleted) return;

	Mesh->SetVisibility(true);
	Mesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void AAshley_MinivanPuzzle::Click_Implementation()
{
	OnPuzzleClicked.Broadcast(this);
}

void AAshley_MinivanPuzzle::CompletePuzzle()
{
	bIsPuzzleCompleted = true;

	// Aggiungo gli indizii da poter posizionare nella board
	auto Character = Cast<AAshley_CharacterPlayer>(GetWorld()->GetFirstPlayerController()->GetCharacter());
	
	if (bGivesHint)
	{
		for (auto& Name : HintsNames)
		{
			Character->GetInventory()->PutObjectInventory(Name, EPickupType::COLLECTABLE);
		}
	}
}

void AAshley_MinivanPuzzle::StartPuzzleEvent()
{
	StartPuzzle();
}

void AAshley_MinivanPuzzle::ExitPuzzleEvent()
{
	ExitPuzzle();
}

// Not wrapped in WITH_EDITOR: these are UFUNCTIONs, so UHT generates exec thunks in every build configuration.
TArray<FName> AAshley_MinivanPuzzle::GetValidHintIDs() const
{
	UAshley_PickupDA* LoadedAsset;
	if (HintsDataAsset.IsValid())
	{
		LoadedAsset = HintsDataAsset.Get();
	}
	else
	{
		LoadedAsset = HintsDataAsset.LoadSynchronous();
	}
	TArray<FName> IDs;
	if (LoadedAsset)
	{
		IDs = LoadedAsset->GetIDsValids();
	}
	return IDs;
}

TArray<FName> AAshley_MinivanPuzzle::GetValidPickupIDs() const
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

