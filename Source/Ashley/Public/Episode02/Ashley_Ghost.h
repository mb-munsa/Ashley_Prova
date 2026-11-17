// Ashley

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Ashley_Ghost.generated.h"

class AAshley_FarmerPlayer;
class USphereComponent;
class USplineComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGhostAggroed);

UENUM(BlueprintType)
enum class EGhostStatus : uint8
{
	Patrolling,
	Standing,
	Aggroed,
	Attacking
};

UCLASS()
class ASHLEY_API AAshley_Ghost : public AActor
{
	GENERATED_BODY()
	
public:

	UPROPERTY(BlueprintAssignable)
	FOnGhostAggroed OnGhostAggroed;

	// Sets default values for this actor's properties
	AAshley_Ghost();
	virtual void Tick(float DeltaTime) override;

	void SetParameters(float NewPatrolSpeed, float NewChaseSpeed, EGhostStatus InitialStatus, USplineComponent* PatrolPath, float NewChaseDuration, float NewSpawnDuration = 30);
	void SetParameters(float NewPatrolSpeed, float NewChaseSpeed, EGhostStatus InitialStatus, float NewChaseDuration, float NewSpawnDuration = 30);

	UFUNCTION()
	void TriggerAggro(AAshley_FarmerPlayer* FarmerCharacter);

	UFUNCTION(BlueprintImplementableEvent)
	void Disappear();

	UFUNCTION()
	void ToggleMovement();

	UFUNCTION()
	EGhostStatus GetGhostStatus();

protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void OnSphereOverlapEnd(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex);

	UFUNCTION()
	virtual void OnSphereOverlapBegin(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);

	UFUNCTION()
	void FollowPlayer(float DeltaTime);
	UFUNCTION()
	void AttackPlayer();
	UFUNCTION()
	void Patrol(float DeltaTime);

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Data|Spawn")
	float PatrolMoveSpeed;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Data|Spawn")
	float SpawnDuration;
	UPROPERTY(BlueprintReadWrite, Category = "Data|Spawn")
	float DistanceAlongSpline;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	float AttackRange = 50;
	UPROPERTY(BlueprintReadWrite, Category = "Data|Spawn")
	USplineComponent* Spline;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Data|Spawn")
	float AggroDuration = 8.0f;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Data|Spawn")
	float AggroMoveSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data|Spawn")
	USoundBase* AggroSFX;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Data|Spawn")
	UAudioComponent* AudioSource;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Data|Spawn")
	USphereComponent* ScareCollider;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Data|Spawn")
	USphereComponent* AggroCollider;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	USceneComponent* NewRootComponent;

	float CurrentTimer = 0;
	bool bIsInScareRange;
	bool bIsInitialized;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	EGhostStatus CurrentStatus = EGhostStatus::Standing;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	AAshley_FarmerPlayer* CharacterRef;
};
