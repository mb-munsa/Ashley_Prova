// Ashley


#include "Episode02/Ashley_ScarecrowSpawnArea.h"
#include "Episode02/Ashley_ScarecrowSpawner.h"
#include "Components/SplineComponent.h"

#include <Kismet/GameplayStatics.h>

#include "DebugHelper.h"

// Sets default values
AAshley_ScarecrowSpawnArea::AAshley_ScarecrowSpawnArea()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AAshley_ScarecrowSpawnArea::BeginPlay()
{
	Super::BeginPlay();

	if (SpawnType == ESpawnTypeScarecrow::RUN)
		RunSpline = this->FindComponentByClass<USplineComponent>();
	
}

void AAshley_ScarecrowSpawnArea::OnDirectionalTriggerEnter()
{
	ScarecrowSpawner->OnAreaEntered(this);
}

void AAshley_ScarecrowSpawnArea::SpawnScarecrow()
{
	// Devo stand spawno ghostAtLocation
	if (SpawnType == ESpawnTypeScarecrow::STAND)
	{
		ScarecrowSpawner->SpawnScarecrowAtTransform(this, SpawnDuration);
	}
	// Altrimenti spawno ghost dandogli la spline
	else
	{
		ScarecrowSpawner->SpawnScarecrowAlongSpline(RunSpline, SpawnDuration);
	}
}

