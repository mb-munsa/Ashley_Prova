// Ashley


#include "Core/Ashley_Checkpoint.h"
#include "Components/BoxComponent.h"
#include <Characters/Players/Ashley_CharacterPlayer.h>
#include <Core/Ashley_PlayerController.h>


AAshley_Checkpoint::AAshley_Checkpoint()
{
	PrimaryActorTick.bCanEverTick = false;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));

	TriggerVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxVolume"));
	TriggerVolume->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AAshley_Checkpoint::BeginPlay()
{
	Super::BeginPlay();

	// Aggiunto tutti i Box come trigger
	TArray<UBoxComponent*> TriggerVolumes;
	this->GetComponents<UBoxComponent>(TriggerVolumes);

	for (const auto& trigger : TriggerVolumes)
	{
		trigger->OnComponentBeginOverlap.AddDynamic(this, &AAshley_Checkpoint::OnBoxOverlap);
	}
}

void AAshley_Checkpoint::OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!bIsActive) return;

	AAshley_CharacterPlayer* Character = Cast<AAshley_CharacterPlayer>(OtherActor);
	AAshley_PlayerController* PlayerRef = Cast<AAshley_PlayerController>(Character->GetController());

	if (!PlayerRef) return;

	//PlayerRef->RegisterCheckpoint(SaveData);
}

