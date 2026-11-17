// Ashley

#pragma once

#include "CoreMinimal.h"
#include "NativeGameplayTags.h"
#include "Characters/Players/Ashley_CharacterPlayer.h"

#include "Ashley_FarmerPlayer.generated.h"

class UAshley_HeartRateComponent;
class UAshley_InventoryUI;
class AAshley_GhostSpawner;
class AAshley_ScarecrowSpawner;
class UAudioComponent;

UCLASS()
class ASHLEY_API AAshley_FarmerPlayer : public AAshley_CharacterPlayer
{
	GENERATED_BODY()

public:
	AAshley_FarmerPlayer();

	virtual void BeginPlay() override;

	void PossessedBy(AController* NewController) override;

	UFUNCTION(BlueprintCallable)
	void FarmerTriggerScare(bool bShouldBeScared);

	UFUNCTION(BlueprintCallable)
	void FarmerHeartAttack();

	UFUNCTION(BlueprintCallable)
	void OnHeartAttackAnimEnded();

	UFUNCTION(BlueprintCallable)
	void LightFire();

	UFUNCTION(BlueprintCallable)
	void Die();

	void SetGhostSpawner(AAshley_GhostSpawner* Spawner);

	void SetScarecrowSpawner(AAshley_ScarecrowSpawner* Spawner);

	UFUNCTION()
	void OnPlayerEventTriggered(FGameplayTag EventName, FString EventParameter);

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player|Heart")
	UAshley_HeartRateComponent* HeartRateComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player|Heart")
	AAshley_GhostSpawner* GhostSpawner;

	/*UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player|Heart")
	int HeartPillsNumber;*/

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player|Heart")
	int HeartPillsInitialNumber;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player|Heart")
	FName HeartPillID;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player|Heart")
	UAnimMontage* HeartAttackMontage;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player|Heart")
	UAudioComponent* AudioSourceHeart;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player|Heart")
	UAudioComponent* AudioSourceBreath;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player|Animations")
	TObjectPtr<UAnimMontage> LightFireMontage;

	// TODO: temp
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data|Inventory")
	TSubclassOf<UAshley_InventoryUI> DiaryBP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	FGameplayTag PlayerEnteredHouseEvent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	FGameplayTag SequenceEndEvent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	FGameplayTag EventEquipTorch;


	virtual void Sprint(bool bSprint) override;
	
};
