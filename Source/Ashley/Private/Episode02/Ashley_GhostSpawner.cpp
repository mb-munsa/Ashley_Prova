// Ashley


#include "Episode02/Ashley_GhostSpawner.h"
#include "Episode02/Ashley_Ghost.h"
#include "Episode02/Ashley_GhostSpawnArea.h"
#include "Characters/Players/Ashley_FarmerPlayer.h"
#include "Components/SplineComponent.h"
#include <Kismet/GameplayStatics.h>

#include "Core/Ashley_TriggerCollider.h"

#include "DebugHelper.h"
#include <Managers/Ashley_EventManagerSystem.h>
#include <Managers/Ashley_WidgetSystem.h>

// Sets default values
AAshley_GhostSpawner::AAshley_GhostSpawner()
{
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AAshley_GhostSpawner::BeginPlay()
{
	Super::BeginPlay();

	// Recupero character ref
	CharacterRef = Cast<AAshley_FarmerPlayer>(UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetCharacter());
	if (CharacterRef) CharacterRef->SetGhostSpawner(this);

	auto EventManager = UGameplayStatics::GetGameInstance(this)->GetSubsystem<UAshley_EventManagerSystem>();
	EventManager->OnEventTriggered.AddDynamic(this, &AAshley_GhostSpawner::OnPlayerEventTriggered);

	CurrentSpawnProbability = InitialSpawnProbability;
}

void AAshley_GhostSpawner::OnPlayerEventTriggered(FGameplayTag EventName, FString EventParameter)
{
	if (EventName == PlayerEnteredHouseEvent)
	{
		bIsPlayerInHouse = !bIsPlayerInHouse;

		if (bIsPlayerInHouse) StopGhost();
	}
	else if (EventName == StartSequenceEvent)
	{
		StopGhost();
	}
}

void AAshley_GhostSpawner::OnGhostAggroed()
{
	//Debug::Print("On ghost aggroed");

	CurrentRemainingCooldown = CooldownAfterAttack + AggroDuration;

	// Ricordo al player che può scattare
	UGameplayStatics::GetGameInstance(this)->
		GetSubsystem<UAshley_WidgetSystem>()->ShowHint("Press SHIFT to Run");
}

void AAshley_GhostSpawner::OnAreaEntered(AAshley_GhostSpawnArea* SpawnArea)
{
	//Debug::Print("OnAreaEntered");
	if (bIsDisabled) return;

	// Se il ghost è in cooldown non faccio niente
	if (CurrentRemainingCooldown > 0) return;

	//Debug::Print("CurrentRemainingCooldown == 0");

	// Se il ghost è già aggroed non faccio niente
	if (IsValid(GhostInstance) && GhostInstance->GetGhostStatus() == EGhostStatus::Aggroed) return;

	// Se il ghost è già attivo in questa area non faccio niente
	if (IsValid(GhostInstance) && LastSpawnArea == SpawnArea) return;

	//Debug::Print("Ghost not aggroed");

	// TODO:
	int SpawnGhostPerc = FMath::RandRange(0, 100);
	bool bSpawnGhost = SpawnGhostPerc <= CurrentSpawnProbability;

	if (bSpawnGhost)
	{
		//Debug::Print("Spawnign GHOST");

		if (IsValid(GhostInstance)) StopGhost();

 		SpawnArea->SpawnGhost();
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

void AAshley_GhostSpawner::SpawnGhostAtLocation(AActor* SpawnLocation, float SpawnDuration, bool bIsAggroed)
{
	if (bIsDisabled) return;

	StopGhost();

	GhostInstance = GetWorld()->SpawnActor<AAshley_Ghost>(GhostBlueprint, SpawnLocation->GetActorLocation(), SpawnLocation->GetActorRotation());
	GhostInstance->OnGhostAggroed.AddDynamic(this, &AAshley_GhostSpawner::OnGhostAggroed);
	if (bIsAggroed) GhostInstance->TriggerAggro(CharacterRef);
	GhostInstance->SetParameters(GhostPatrollingMoveSpeed, GhostChaseMoveSpeed, bIsAggroed ? EGhostStatus::Aggroed : EGhostStatus::Standing, SpawnDuration);
	CurrentRemainingCooldown = CooldownBetweenAreas;
}

void AAshley_GhostSpawner::SpawnGhostAtTransform(AActor* Transform, float SpawnDuration)
{
	if (bIsDisabled) return;

	GhostInstance = GetWorld()->SpawnActor<AAshley_Ghost>(GhostBlueprint, Transform->GetActorLocation(), Transform->GetActorRotation());
	GhostInstance->SetParameters(GhostPatrollingMoveSpeed, GhostChaseMoveSpeed, EGhostStatus::Standing, AggroDuration, SpawnDuration);
	GhostInstance->OnGhostAggroed.AddDynamic(this, &AAshley_GhostSpawner::OnGhostAggroed);
}

void AAshley_GhostSpawner::SpawnGhostAlongSpline(USplineComponent* SplineComponent, float SpawnDuration)
{
	if (bIsDisabled) return;

	// Decidere start patrol
	auto SpawnPoint = SplineComponent->GetLocationAtDistanceAlongSpline(0.0f, ESplineCoordinateSpace::World);

	GhostInstance = GetWorld()->SpawnActor<AAshley_Ghost>(GhostBlueprint, SpawnPoint, FRotator::ZeroRotator);
	GhostInstance->SetParameters(GhostPatrollingMoveSpeed, GhostChaseMoveSpeed, EGhostStatus::Patrolling, SplineComponent, AggroDuration, SpawnDuration);
	GhostInstance->OnGhostAggroed.AddDynamic(this, &AAshley_GhostSpawner::OnGhostAggroed);
}

void AAshley_GhostSpawner::StopGhost()
{
	if (IsValid(GhostInstance))
	{
		GhostInstance->ToggleMovement();
		GhostInstance->Disappear();
	}
	GhostInstance = nullptr;
}

void AAshley_GhostSpawner::DisableGhost()
{
	StopGhost();
	bIsDisabled = true;
}

void AAshley_GhostSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (CurrentRemainingCooldown == 0) return;

	CurrentRemainingCooldown = FMath::Max(CurrentRemainingCooldown - DeltaTime, 0);
}
