// Ashley

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "NativeGameplayTags.h"

#include "Core/Ashley_Poltergeist.h"

#include "Ashley_PoltergeistManager.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLevelIncrement, int, NewLevelIndex);


UENUM(BlueprintType)
enum class ELevelUpdateType : uint8
{
	TIMER,
	GENERIC_EVENT,
	ITEM_PICKUP
};

USTRUCT(BlueprintType)
struct FLevelUpdateCondition
{
	GENERATED_BODY()

public:

	// Condizione per passare a questo livello
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Data")
	ELevelUpdateType UpdateType = ELevelUpdateType::GENERIC_EVENT;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Data", meta = (EditCondition = "UpdateType == ELevelUpdateType::TIMER", EditConditionHides))
	float Timer = 0.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Data", meta = (EditCondition = "UpdateType == ELevelUpdateType::ITEM_PICKUP", EditConditionHides))
	FName ItemToPick;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Data", meta = (EditCondition = "UpdateType == ELevelUpdateType::GENERIC_EVENT", EditConditionHides))
	FGameplayTag EventName;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Data", meta = (EditCondition = "UpdateType == ELevelUpdateType::GENERIC_EVENT", EditConditionHides))
	FString EventParam;
};

USTRUCT(BlueprintType)
struct FFearLevel
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Data")
	TArray<FLevelUpdateCondition> UpdateConditions;
};

/**
 * 
 */
UCLASS()
class ASHLEY_API UAshley_PoltergeistManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:

	// Evento lanciato al passaggio al livello successivo
	UPROPERTY(BlueprintAssignable)
	FOnLevelIncrement OnLevelIncrement;

	UFUNCTION()
	void InitMap(UAshley_MapFearLevels* Levels);

	UFUNCTION(BlueprintCallable)
	void PlayerTriggerCollider(AAshley_Poltergeist* Poltergeist) const;

	UFUNCTION()
	void OnPlayerEventTriggered(FGameplayTag EventName, FString EventParameter);

	UFUNCTION()
	void UpdateLevel();

	UFUNCTION(BlueprintCallable)
	int GetCurrentLevel() const;
	
protected:

	// Livelli di pericolosità della mappa
	UPROPERTY()
	TArray<FFearLevel> FearLevels;

	// Livello di pericolosità corrente
	UPROPERTY()
	int CurrentLevelIndex;

	UPROPERTY()
	float CurrentTimer;

	UPROPERTY()
	bool bIsTimerActive;

	// Minimo intervallo tra due poltergeist importanti?
	UPROPERTY()
	float MinPoltergeistInterval;

	UPROPERTY()
	FTimerHandle TimerHandle_NextLevel;
	
};
