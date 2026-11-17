// Ashley

#pragma once

#include "CoreMinimal.h"
#include "NativeGameplayTags.h"

#include "Ashley_FarmerLevelManager.generated.h"

class AAshley_TriggerCollider;
class AAshley_GhostSpawner;
class AAshley_Door;
class UAshley_MapFearLevels;
class UAshley_GenericDialogue;
class UAshley_PickupDA;
class AAshley_GhostWife;
class UAshley_PoltergeistManager;

/**
 * 
 */
UCLASS()
class ASHLEY_API AAshley_FarmerLevelManager : public AActor
{
	GENERATED_BODY()

public:
	AAshley_FarmerLevelManager();

	UFUNCTION()
	void OnPlayerEventTriggered(FGameplayTag EventName, FString EventParam);

	UFUNCTION()
	void OnItemPickupHandler(FName ItemID);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	UAshley_MapFearLevels* FearLevels;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	bool bHasShowedRunPrompt = false;

	UFUNCTION()
	TArray<FName> GetValidIDs() const;

	UPROPERTY(EditAnywhere, Category = "Data")
	TSoftObjectPtr<UAshley_PickupDA> PickupDataAsset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	FGameplayTag EventEndSequence;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	FGameplayTag EventStartSequence;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	FGameplayTag EventEnterHouse;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	FGameplayTag EventEquipTorch;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data|Wife")
	int MinFearLevel = 2;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Data|Wife")
	bool bHasSpawnedWife = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data|Wife")
	FGameplayTag WifeAppearSequenceIndex;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data|Wife")
	TSubclassOf<AAshley_GhostWife> GhostWifePrefab;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data|Wife")
	TObjectPtr<AActor> GhostWifeSpawn;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data|TelephoneSequence")
	FGameplayTag TelephoneSequenceIndex;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data|TelephoneSequence")
	UAshley_GenericDialogue* TelephoneDialogue;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data|BlackHoleSequence")
	FGameplayTag BlackHoleSequenceIndex;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data|BlackHoleSequence", meta = (GetOptions = "GetValidIDs"))
	FName HeartPill;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data|BlackHoleSequence")
	UAshley_GenericDialogue* BlackHoleDialogue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data|BlackHoleSequence")
	int32 InitialPillsNumber = 3;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data|BlackHoleSequence")
	TObjectPtr<AAshley_Door> HouseDoor;

	bool bHasSequenceDoorPlayed = false;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data|BarnSequence", meta = (GetOptions = "GetValidIDs"))
	FName PetrolTank;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Data|BarnSequence")
	AAshley_TriggerCollider* SecondSequenceTrigger;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data|BarnSequence")
	FGameplayTag BarnSequenceIndex;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data|BarnSequence")
	AAshley_GhostSpawner* GhostSpawner;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data|BarnSequence")
	AActor* GhostSpawnPointBarn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data|BarnSequence")
	float GhostDuration = 15;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data|GarageSequence")
	FGameplayTag GarageSequenceIndex;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data|GarageSequence")
	AActor* GhostSpawnPointGarage;

protected:
	virtual void BeginPlay() override;

	UAshley_PoltergeistManager* PoltergeistManager;
};
