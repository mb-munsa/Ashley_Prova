// Ashley


#include "Episode02/Ashley_GhostSpawnArea.h"
#include "Episode02/Ashley_GhostSpawner.h"
#include "Components/SplineComponent.h"

#include <Kismet/GameplayStatics.h>

#include "DebugHelper.h"

// Sets default values
AAshley_GhostSpawnArea::AAshley_GhostSpawnArea()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

// Called when the game starts or when spawned
void AAshley_GhostSpawnArea::BeginPlay()
{
	Super::BeginPlay();

	if (SpawnType == ESpawnType::PATROL)
		PatrolSpline = this->FindComponentByClass<USplineComponent>();
}

void AAshley_GhostSpawnArea::OnDirectionalTriggerEnter()
{
	GhostSpawner->OnAreaEntered(this);
}

void AAshley_GhostSpawnArea::SpawnGhost()
{
	// Devo stand spawno ghostAtLocation
	if (SpawnType == ESpawnType::STAND)
	{
		GhostSpawner->SpawnGhostAtTransform(this, SpawnDuration);
	}
	// Altrimenti spawno ghost dandogli la spline
	else
	{
		GhostSpawner->SpawnGhostAlongSpline(PatrolSpline, SpawnDuration);
	}
}

