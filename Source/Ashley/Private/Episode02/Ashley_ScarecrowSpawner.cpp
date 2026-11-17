// Ashley


#include "Episode02/Ashley_ScarecrowSpawner.h"
#include "Episode02/Ashley_ScarecrowSpawnArea.h"
#include "Characters/Players/Ashley_FarmerPlayer.h"
#include "Components/SplineComponent.h"
#include <Kismet/GameplayStatics.h>

#include "Core/Ashley_TriggerCollider.h"

#include "DebugHelper.h"
#include <Managers/Ashley_EventManagerSystem.h>
#include <Managers/Ashley_WidgetSystem.h>

// Sets default values
AAshley_ScarecrowSpawner::AAshley_ScarecrowSpawner()
{
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AAshley_ScarecrowSpawner::BeginPlay()
{
	Super::BeginPlay();
	
	// Recupero character ref
	CharacterRef = Cast<AAshley_FarmerPlayer>(UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetCharacter());
	if (CharacterRef) CharacterRef->SetScarecrowSpawner(this);

	auto EventManager = UGameplayStatics::GetGameInstance(this)->GetSubsystem<UAshley_EventManagerSystem>();
	EventManager->OnEventTriggered.AddDynamic(this, &AAshley_ScarecrowSpawner::OnPlayerEventTriggered);

	CurrentSpawnProbability = InitialSpawnProbability;

}

void AAshley_ScarecrowSpawner::OnPlayerEventTriggered(FGameplayTag EventName, FString EventParameter)
{
	if (EventName == PlayerEnteredHouseEvent)
	{
		bIsPlayerInHouse = !bIsPlayerInHouse;

		if (bIsPlayerInHouse) StopScarecrow();
	}
	else if (EventName == StartSequenceEvent)
	{
		StopScarecrow();
	}
}

void AAshley_ScarecrowSpawner::OnAreaEntered(AAshley_ScarecrowSpawnArea* SpawnArea)
{
	//Debug::Print("OnAreaEntered");
	if (bIsDisabled) return;

	// Se il scarecrow è in cooldown non faccio niente
	if (CurrentRemainingCooldown > 0) return;

	//Debug::Print("CurrentRemainingCooldown == 0");

	// Se il scarecrow è già attivo in questa area non faccio niente
	if (IsValid(ScarecrowInstance) && LastSpawnArea == SpawnArea) return;

	//Debug::Print("Ghost not aggroed");

	// TODO:
	int SpawnScarecrowPerc = FMath::RandRange(0, 100);
	bool bSpawnScarecrow = SpawnScarecrowPerc <= CurrentSpawnProbability;

	if (bSpawnScarecrow)
	{
		//Debug::Print("Spawnign GHOST");

		if (IsValid(ScarecrowInstance)) StopScarecrow();

		SpawnArea->SpawnScarecrow();
		LastSpawnArea = SpawnArea;

		// abbasso probabilità
		CurrentSpawnProbability = FMath::Clamp(CurrentSpawnProbability - SpawnProbabilityDecrement, 0, 100);
	}
	else
	{
		// alzo probabilità
		CurrentSpawnProbability = FMath::Clamp(CurrentSpawnProbability + SpawnProbabilityIncrement, 0, 100);
	}

	// In ogni caso aggiungo un piccolo cooldown tra un'area e la prossima
	CurrentRemainingCooldown += CooldownBetweenAreas;
}

void AAshley_ScarecrowSpawner::SpawnScarecrowAtTransform(AActor* Transform, float SpawnDuration)
{
	if (bIsDisabled) return;

	ScarecrowInstance = GetWorld()->SpawnActor<AAshley_Scarecrow>(ScarecrowBlueprint, Transform->GetActorLocation(), Transform->GetActorRotation());
}

void AAshley_ScarecrowSpawner::SpawnScarecrowAlongSpline(USplineComponent* SplineComponent, float SpawnDuration)
{
	if (bIsDisabled) return;

	// Decidere start patrol
	auto SpawnPoint = SplineComponent->GetLocationAtDistanceAlongSpline(0.0f, ESplineCoordinateSpace::World);

	ScarecrowInstance = GetWorld()->SpawnActor<AAshley_Scarecrow>(ScarecrowBlueprint, SpawnPoint, FRotator::ZeroRotator);
}

void AAshley_ScarecrowSpawner::StopScarecrow()
{
	if (IsValid(ScarecrowInstance))
	{
		ScarecrowInstance->ToggleMovement();
		ScarecrowInstance->Disappear();
	}
	ScarecrowInstance = nullptr;
}

void AAshley_ScarecrowSpawner::DisableScarecrow()
{
	StopScarecrow();
	bIsDisabled = true;
}

// Called every frame
void AAshley_ScarecrowSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

