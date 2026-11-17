// Ashley


#include "Core/Ashley_Poltergeist.h"

#include "DataAssets/Ashley_PoltergeistDA.h"
#include "Characters/Players/Ashley_CharacterPlayer.h"
#include "Managers/Ashley_PoltergeistManager.h"
#include "Components/BoxComponent.h"
#include <Kismet/GameplayStatics.h>

// Sets default values
AAshley_Poltergeist::AAshley_Poltergeist()
{
	MeshBase = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshBase"));
	MeshBase->SetupAttachment(RootComponent);

	MeshBase->SetRenderCustomDepth(true);
	MeshBase->SetCustomDepthStencilValue(0);

	BoxProximity = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxProximity"));
	BoxProximity->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AAshley_Poltergeist::BeginPlay()
{
	Super::BeginPlay();

	BoxProximity->OnComponentBeginOverlap.AddDynamic(this, &AAshley_Poltergeist::OnBoxProximityOverlap);
}

uint8 AAshley_Poltergeist::GetActivationLevel() const
{
	return ActivationLevel;
}

bool AAshley_Poltergeist::IsOneShot() const
{
	return Frequency == EPoltergeistFrequency::ONE_SHOT;
}

EPoltergeistActivationType AAshley_Poltergeist::GetActivationType()
{
	return ActivationType;
}

double AAshley_Poltergeist::GetCooldown() const
{
	return Cooldown;
}

uint8 AAshley_Poltergeist::GetActivationProbability() const
{
	return ActivationProbability;
}

double AAshley_Poltergeist::GetLastActivationTime() const
{
	return LastActivationTime;
}

bool AAshley_Poltergeist::HasBeenPlayed() const
{
	return bHasBeenPlayed;
}

void AAshley_Poltergeist::Activate_Implementation(int FearLevel)
{
	bHasBeenPlayed = true;
	LastActivationTime = GetWorld()->GetTimeSeconds();
}

void AAshley_Poltergeist::OnBoxProximityOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	auto Character = Cast<AAshley_CharacterPlayer>(OtherActor);

	// Notifico il manager che il player è entrato nell'area
	if (Character)
		UGameplayStatics::GetGameInstance(this)->GetSubsystem<UAshley_PoltergeistManager>()->PlayerTriggerCollider(this);
}

