// Ashley

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Ashley_Scarecrow.generated.h"

class AAshley_CharacterPlayer;
class USphereComponent;
class USplineComponent;

UENUM(BlueprintType)
enum class EScarecrowStatus : uint8
{
	Run,
	Standing,
	Watch
};

UCLASS()
class ASHLEY_API AAshley_Scarecrow : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAshley_Scarecrow();
	virtual void Tick(float DeltaTime) override;

	void SetParameters(float NewRunSpeed, EScarecrowStatus InitialStatus, USplineComponent* RunPath, float NewSpawnDuration = 30);
	void SetParameters(float NewRunSpeed, EScarecrowStatus InitialStatus, float NewSpawnDuration = 30);

	UFUNCTION(BlueprintImplementableEvent)
	void Disappear();

	UFUNCTION()
	void ToggleMovement();

	UFUNCTION()
	EScarecrowStatus GetScarecrowStatus();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UFUNCTION()
	void Run(float DeltaTime);
	UFUNCTION()
	void Watch(float DeltaTime);

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Data|Spawn")
	float RunMoveSpeed;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Data|Spawn")
	float SpawnDuration;
	UPROPERTY(BlueprintReadWrite, Category = "Data|Spawn")
	float DistanceAlongSpline;
	UPROPERTY(BlueprintReadWrite, Category = "Data|Spawn")
	USplineComponent* Spline;

	//UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Data|Spawn")
	//UAudioComponent* AudioSource;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Data|Spawn")
	USphereComponent* ScareCollider;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	USceneComponent* NewRootComponent;

	float CurrentTimer = 0;
	bool bIsInScareRange;
	bool bIsInitialized;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	EScarecrowStatus CurrentStatus = EScarecrowStatus::Standing;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	AAshley_CharacterPlayer* CharacterRef;

};
