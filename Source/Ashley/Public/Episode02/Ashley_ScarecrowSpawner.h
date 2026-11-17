// Ashley

#pragma once

#include "CoreMinimal.h"
#include "NativeGameplayTags.h"
#include "GameFramework/Actor.h"
#include "Ashley_ScarecrowSpawner.generated.h"

class AAshley_FarmerPlayer;
class AAshley_Scarecrow;
class AAshley_ScarecrowSpawnArea;
class USplineComponent;

UCLASS()
class ASHLEY_API AAshley_ScarecrowSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAshley_ScarecrowSpawner();
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void StopScarecrow();

	UFUNCTION()
	void DisableScarecrow();

	UFUNCTION()
	void SpawnScarecrowAtTransform(AActor* Transform, float SpawnDuration);

	UFUNCTION()
	void SpawnScarecrowAlongSpline(USplineComponent* SplineComponent, float SpawnDuration);

	UFUNCTION(BlueprintCallable)
	void OnAreaEntered(AAshley_ScarecrowSpawnArea* AreaIndex);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnPlayerEventTriggered(FGameplayTag EventName, FString EventParam);

	// Parameters
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data|Parameters")
	float SpawnProbabilityIncrement = 50;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data|Parameters")
	float SpawnProbabilityDecrement = 50;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data|Parameters")
	float InitialSpawnProbability = 50;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data|Parameters")
	float CooldownBetweenAreas = 10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data|Parameters")
	float ScarecrowMoveSpeed = 40;

	// References
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data|References")
	FGameplayTag PlayerEnteredHouseEvent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data|References")
	FGameplayTag StartSequenceEvent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data|References")
	TSubclassOf<AAshley_Scarecrow> ScarecrowBlueprint;

	
	UPROPERTY()
	AAshley_FarmerPlayer* CharacterRef;
	UPROPERTY()
	AAshley_Scarecrow* ScarecrowInstance = nullptr;
	UPROPERTY()
	AAshley_ScarecrowSpawnArea* LastSpawnArea;

	float CurrentSpawnProbability;
	float CurrentRemainingCooldown;

	bool bIsPlayerInHouse = true;
	bool bIsDisabled = false;
};
