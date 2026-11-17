// Ashley

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Ashley_ScarecrowSpawnArea.generated.h"

class USplineComponent;
class AAshley_ScarecrowSpawner;

UENUM(BlueprintType)
enum class ESpawnTypeScarecrow : uint8
{
	STAND UMETA(DisplayName = "Stand"),
	WATCH UMETA(DisplayName = "Watch"),
	RUN UMETA(DisplayName = "Run")
};

UCLASS()
class ASHLEY_API AAshley_ScarecrowSpawnArea : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAshley_ScarecrowSpawnArea();

	UFUNCTION(BlueprintCallable)
	void OnDirectionalTriggerEnter();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	ESpawnTypeScarecrow SpawnType;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Data")
	USplineComponent* RunSpline;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	AAshley_ScarecrowSpawner* ScarecrowSpawner;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	float SpawnDuration;

	UFUNCTION()
	void SpawnScarecrow();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

};
