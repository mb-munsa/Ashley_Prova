// Ashley

#pragma once

#include "CoreMinimal.h"
#include "NativeGameplayTags.h"
#include "GameFramework/Actor.h"
#include "Ashley_GhostSpawner.generated.h"

class AAshley_FarmerPlayer;
class AAshley_Ghost;
class AAshley_GhostSpawnArea;
class USplineComponent;

UCLASS()
class ASHLEY_API AAshley_GhostSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	AAshley_GhostSpawner();
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void StopGhost();

	UFUNCTION()
	void DisableGhost();

	UFUNCTION()
	void SpawnGhostAtLocation(AActor* SpawnPosition, float SpawnDuration, bool bIsAggroed);

	UFUNCTION()
	void SpawnGhostAtTransform(AActor* Transform, float SpawnDuration);

	UFUNCTION()
	void SpawnGhostAlongSpline(USplineComponent* SplineComponent, float SpawnDuration);

	UFUNCTION(BlueprintCallable)
	void OnAreaEntered(AAshley_GhostSpawnArea* AreaIndex);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnPlayerEventTriggered(FGameplayTag EventName, FString EventParam);

	UFUNCTION()
	void OnGhostAggroed();

	// Parameters
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data|Parameters")
	float SpawnProbabilityIncrement = 50;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data|Parameters")
	float SpawnProbabilityDecrement = 50;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data|Parameters")
	float InitialSpawnProbability = 50;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data|Parameters")
	float CooldownAfterAttack = 10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data|Parameters")
	float CooldownBetweenAreas = 10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data|Parameters")
	float GhostPatrollingMoveSpeed = 40;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data|Parameters")
	float GhostChaseMoveSpeed = 60;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data|Parameters")
	float AggroDuration = 10;

	// References
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data|References")
	FGameplayTag PlayerEnteredHouseEvent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data|References")
	FGameplayTag StartSequenceEvent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data|References")
	TSubclassOf<AAshley_Ghost> GhostBlueprint;


	UPROPERTY()
	AAshley_FarmerPlayer* CharacterRef;
	UPROPERTY()
	AAshley_Ghost* GhostInstance = nullptr;
	UPROPERTY()
	AAshley_GhostSpawnArea* LastSpawnArea;

	float CurrentSpawnProbability;
	float CurrentRemainingCooldown;

	bool bIsPlayerInHouse = true;
	bool bIsDisabled = false;
};
