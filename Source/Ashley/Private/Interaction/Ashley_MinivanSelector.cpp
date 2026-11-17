// Ashley


#include "Interaction/Ashley_MinivanSelector.h"

AAshley_MinivanSelector::AAshley_MinivanSelector()
{
	PrimaryActorTick.bCanEverTick = false;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);
}

void AAshley_MinivanSelector::Click_Implementation()
{
	// Se qualcuno gestisce il click (es. PuzzleSelector), decide lui quando cambiare vista
	if (OnSelectorClicked.IsBound())
	{
		OnSelectorClicked.Broadcast(this);
		return;
	}

	if (Manager)
		Manager->SwitchToView(SceneToFocus);
}

void AAshley_MinivanSelector::SetCollisionEnabled(bool bEnabled)
{
	Mesh->SetCollisionEnabled(bEnabled ? ECollisionEnabled::QueryOnly : ECollisionEnabled::NoCollision);
}
