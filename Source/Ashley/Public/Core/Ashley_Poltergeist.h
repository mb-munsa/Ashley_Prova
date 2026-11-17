// Ashley

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <DataAssets/Ashley_PoltergeistDA.h>
#include "Interaction/Ashley_ActorInteract.h"

#include "Ashley_Poltergeist.generated.h"

class UBoxComponent;

UCLASS()
class ASHLEY_API AAshley_Poltergeist : public AAshley_ActorInteract
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAshley_Poltergeist();

	UFUNCTION(BlueprintNativeEvent)
	void Activate(int FearLevel);

	UFUNCTION()
	uint8 GetActivationLevel() const;

	UFUNCTION()
	EPoltergeistActivationType GetActivationType();

	UFUNCTION()
	bool IsOneShot() const;

	UFUNCTION()
	double GetLastActivationTime() const;

	UFUNCTION()
	double GetCooldown() const;

	UFUNCTION()
	uint8 GetActivationProbability() const;

	UFUNCTION()
	bool HasBeenPlayed() const;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnBoxProximityOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data|Type")
	EPoltergeistActivationType ActivationType = EPoltergeistActivationType::ALWAYS_ACTIVE;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data|Type")
	EPoltergeistFrequency Frequency = EPoltergeistFrequency::ONE_SHOT;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data|Type")
	uint8 ActivationLevel = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data|Type", meta = (EditCondition = "ActivationType == EPoltergeistActivationType::ACTIVATE_BY_PROXIMITY_RANDOM", EditConditionHides))
	uint8 ActivationProbability = 50;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data|Type")
	double Cooldown = 60.0;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Data")
	TObjectPtr<UStaticMeshComponent> MeshBase;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Data")
	TObjectPtr<UBoxComponent> BoxProximity;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Data")
	bool bHasBeenPlayed = false;

	double LastActivationTime = -9999;
};
