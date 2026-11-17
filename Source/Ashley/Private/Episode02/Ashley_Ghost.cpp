// Ashley


#include "Episode02/Ashley_Ghost.h"

#include "Characters/Players/Ashley_FarmerPlayer.h"
#include "Components/AudioComponent.h"
#include "Components/SplineComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/KismetMathLibrary.h"

#include "DebugHelper.h"

//FOnGhostAggroed AAshley_Ghost::OnGhostAggroed;

// Sets default values
AAshley_Ghost::AAshley_Ghost()
{
	PrimaryActorTick.bCanEverTick = true;

	NewRootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("NewRootSceneComponent"));
	RootComponent = NewRootComponent;

	AudioSource = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponent"));
	AudioSource->SetupAttachment(NewRootComponent);

	ScareCollider = CreateDefaultSubobject<USphereComponent>(TEXT("ScareCollider"));
	ScareCollider->SetupAttachment(NewRootComponent);

	AggroCollider = CreateDefaultSubobject<USphereComponent>(TEXT("AggroCollider"));
	AggroCollider->SetupAttachment(NewRootComponent);
}

// Called when the game starts or when spawned
void AAshley_Ghost::BeginPlay()
{
	Super::BeginPlay();

	CurrentTimer = 0;
	CurrentStatus = EGhostStatus::Patrolling;
	AudioSource->SetSound(AggroSFX);

	ScareCollider->OnComponentBeginOverlap.AddDynamic(this, &AAshley_Ghost::OnSphereOverlapBegin);
	AggroCollider->OnComponentBeginOverlap.AddDynamic(this, &AAshley_Ghost::OnSphereOverlapBegin);
	ScareCollider->OnComponentEndOverlap.AddDynamic(this, &AAshley_Ghost::OnSphereOverlapEnd);
}

// Called every frame
void AAshley_Ghost::Tick(float DeltaTime)
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
		case EGhostStatus::Patrolling:
			Patrol(DeltaTime);
			break;
		case EGhostStatus::Aggroed:
			FollowPlayer(DeltaTime);
			break;
		case EGhostStatus::Attacking:
			AttackPlayer();
		case EGhostStatus::Standing:
			break;
	}
}

void AAshley_Ghost::ToggleMovement()
{
	bIsInitialized = !bIsInitialized;
	if (!bIsInitialized && CharacterRef) CharacterRef->FarmerTriggerScare(false);
}

void AAshley_Ghost::AttackPlayer()
{
	FVector PlayerLocation = CharacterRef->GetActorLocation();
	FVector MyLocation = GetActorLocation();

	if (FVector::Dist(PlayerLocation, MyLocation) > AttackRange * 2)
	{
		CurrentStatus = EGhostStatus::Aggroed;
		return;
	}
}

void AAshley_Ghost::FollowPlayer(float DeltaTime)
{
	if (!CharacterRef) return;

	FVector PlayerLocation = CharacterRef->GetActorLocation();
	FVector MyLocation = GetActorLocation();

	if (FVector::Dist(PlayerLocation, MyLocation) < AttackRange)
	{
		// TODO: audio scream
		CurrentStatus = EGhostStatus::Attacking;
		return;
	}

	FRotator NewRotation = (UKismetMathLibrary::FindLookAtRotation(MyLocation, PlayerLocation));

	FVector NewLocation = FMath::VInterpConstantTo(GetActorLocation(), CharacterRef->GetActorLocation(), DeltaTime, AggroMoveSpeed);
	SetActorLocationAndRotation(NewLocation, NewRotation);
}

void AAshley_Ghost::Patrol(float DeltaTime)
{
	const float SplineLength = Spline->GetSplineLength();
	DistanceAlongSpline += PatrolMoveSpeed * DeltaTime;

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

void AAshley_Ghost::TriggerAggro(AAshley_FarmerPlayer* FarmerCharacter)
{
	CharacterRef = FarmerCharacter;
	AudioSource->Play();
	CurrentStatus = EGhostStatus::Aggroed;
	CurrentTimer = 0;
	SpawnDuration = AggroDuration;

	OnGhostAggroed.Broadcast();

	//Debug::Print("Trigger Aggro");
}

EGhostStatus AAshley_Ghost::GetGhostStatus()
{
	return CurrentStatus;
}

void AAshley_Ghost::SetParameters(float NewPatrolSpeed, float NewChaseSpeed, EGhostStatus InitialStatus, USplineComponent* PatrolPath, float NewChaseDuration, float NewSpawnDuration)
{
	this->PatrolMoveSpeed = NewPatrolSpeed;
	this->AggroMoveSpeed = NewChaseSpeed;
	this->SpawnDuration = NewSpawnDuration;
	this->AggroDuration = NewChaseDuration;
	this->CurrentStatus = InitialStatus;

	Spline = PatrolPath;

	this->bIsInitialized = true;
}

void AAshley_Ghost::SetParameters(float NewPatrolSpeed, float NewChaseSpeed, EGhostStatus InitialStatus, float NewChaseDuration, float NewSpawnDuration)
{
	this->PatrolMoveSpeed = NewPatrolSpeed;
	this->AggroMoveSpeed = NewChaseSpeed;
	this->SpawnDuration = NewSpawnDuration;
	this->AggroDuration = NewChaseDuration;
	this->CurrentStatus = InitialStatus;

	this->bIsInitialized = true;
}

void AAshley_Ghost::OnSphereOverlapBegin(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	//Debug::Print(TEXT("Overlap"));

	auto Character = Cast<AAshley_FarmerPlayer>(OtherActor);

	if (!Character) return;

	if (OverlappedComponent == ScareCollider)
	{
		bIsInScareRange = true;

		if (CurrentStatus == EGhostStatus::Aggroed || CurrentStatus == EGhostStatus::Attacking)
			Character->FarmerTriggerScare(true);
	}
	else if (OverlappedComponent == AggroCollider)
	{
		if (CurrentStatus == EGhostStatus::Aggroed || CurrentStatus == EGhostStatus::Attacking) return;
		
		TriggerAggro(Character);
		if (bIsInScareRange)
			Character->FarmerTriggerScare(true);
	}
}

void AAshley_Ghost::OnSphereOverlapEnd(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex)
{
	auto Character = Cast<AAshley_FarmerPlayer>(OtherActor);

	if (!Character) return;

	if (OverlappedComponent == ScareCollider)
	{
		bIsInScareRange = false;

		if (CurrentStatus == EGhostStatus::Aggroed || CurrentStatus == EGhostStatus::Attacking)
			Character->FarmerTriggerScare(false);
	}
}
