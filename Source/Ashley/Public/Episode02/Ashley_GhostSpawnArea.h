// Ashley

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Ashley_GhostSpawnArea.generated.h"

class USplineComponent;
class AAshley_GhostSpawner;

UENUM(BlueprintType)
enum class ESpawnType : uint8
{
	STAND UMETA(DisplayName = "Stand"),
	PATROL UMETA(DisplayName = "Patrol")
};

UCLASS()
class ASHLEY_API AAshley_GhostSpawnArea : public AActor
{
	GENERATED_BODY()
	
public:	

	UFUNCTION(BlueprintCallable)
	void OnDirectionalTriggerEnter();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	ESpawnType SpawnType;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Data")
	USplineComponent* PatrolSpline;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	AAshley_GhostSpawner* GhostSpawner;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	float SpawnDuration;

	// Sets default values for this actor's properties
	AAshley_GhostSpawnArea();

	UFUNCTION()
	void SpawnGhost();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
