// Ashley

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Ashley_Checkpoint.generated.h"

class UBoxComponent;

UCLASS(BlueprintType)
class UCheckpointData : public UObject
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector Location = FVector::UpVector;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FRotator Rotation = FRotator::ZeroRotator;

	UFUNCTION(BlueprintCallable)
	void SetCheckpointTransform(AActor* Actor) { Location = Actor->GetActorLocation(); Rotation = Actor->GetActorRotation(); }

	//void SetCheckpointLocation(FVector MyLocation) { Location = Location; }

	// TODO: salvo stato dell'inventario?
};

UCLASS()
class ASHLEY_API AAshley_Checkpoint : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAshley_Checkpoint();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	UBoxComponent* TriggerVolume;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	//CheckpointData SaveData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	bool bIsActive = true;
};
