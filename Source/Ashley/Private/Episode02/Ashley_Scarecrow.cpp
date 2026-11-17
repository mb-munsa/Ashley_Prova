// Ashley


#include "Episode02/Ashley_Scarecrow.h"

#include "Characters/Players/Ashley_FarmerPlayer.h"
#include "Components/AudioComponent.h"
#include "Components/SplineComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/KismetMathLibrary.h"

#include "DebugHelper.h"

// Sets default values
AAshley_Scarecrow::AAshley_Scarecrow()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	NewRootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("NewRootSceneComponent"));
	RootComponent = NewRootComponent;

	//AudioSource = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponent"));
	//AudioSource->SetupAttachment(NewRootComponent);

	//ScareCollider = CreateDefaultSubobject<USphereComponent>(TEXT("ScareCollider"));
	//ScareCollider->SetupAttachment(NewRootComponent);

}

// Called when the game starts or when spawned
void AAshley_Scarecrow::BeginPlay()
{
	Super::BeginPlay();
	
	CurrentTimer = 0;
	CurrentStatus = EScarecrowStatus::Run;
	//AudioSource->SetSound(RunSFX);

	//ScareCollider->OnComponentBeginOverlap.AddDynamic(this, &AAshley_Scarecrow::OnSphereOverlapBegin);
	//ScareCollider->OnComponentEndOverlap.AddDynamic(this, &AAshley_Scarecrow::OnSphereOverlapEnd);
}

// Called every frame
void AAshley_Scarecrow::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!bIsInitialized) return;

	//Debug::Print(FString::Printf(TEXT("CurrentTimer %f"), CurrentTimer));

	CurrentTimer += DeltaTime;
	if (CurrentTimer >= SpawnDuration)
	{
		ToggleMovement();
		Disappear();
	}
	switch (CurrentStatus)
	{
	case EScarecrowStatus::Run:
		Run(DeltaTime);
		break;
	case EScarecrowStatus::Watch:
		Watch(DeltaTime);
		break;
	case EScarecrowStatus::Standing:
		break;
	}
}

void AAshley_Scarecrow::ToggleMovement()
{
	bIsInitialized = !bIsInitialized;
}

void AAshley_Scarecrow::Run(float DeltaTime)
{
	const float SplineLength = Spline->GetSplineLength();
	DistanceAlongSpline += RunMoveSpeed * DeltaTime;

	// Wrap around for closed loop
	DistanceAlongSpline = FMath::Fmod(DistanceAlongSpline, SplineLength);

	FVector NewLocation = Spline->GetLocationAtDistanceAlongSpline(
		DistanceAlongSpline,
		ESplineCoordinateSpace::World
	);

	FRotator NewRotation = Spline->GetRotationAtDistanceAlongSpline(
		DistanceAlongSpline,
		ESplineCoordinateSpace::World
	);

	SetActorLocationAndRotation(NewLocation, NewRotation);
}

void AAshley_Scarecrow::Watch(float DeltaTime)
{
}

EScarecrowStatus AAshley_Scarecrow::GetScarecrowStatus()
{
	return CurrentStatus;
}

void AAshley_Scarecrow::SetParameters(float NewRunSpeed, EScarecrowStatus InitialStatus, USplineComponent* RunPath, float NewSpawnDuration)
{
	this->RunMoveSpeed = NewRunSpeed;
	this->SpawnDuration = NewSpawnDuration;
	this->CurrentStatus = InitialStatus;

	Spline = RunPath;

	this->bIsInitialized = true;
}

void AAshley_Scarecrow::SetParameters(float NewRunSpeed, EScarecrowStatus InitialStatus, float NewSpawnDuration)
{
	this->RunMoveSpeed = NewRunSpeed;
	this->SpawnDuration = NewSpawnDuration;
	this->CurrentStatus = InitialStatus;

	this->bIsInitialized = true;
}

