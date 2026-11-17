// Ashley

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NativeGameplayTags.h"
#include "Ashley_SequenceManager.generated.h"

class ULevelSequence;
class ULevelSequencePlayer;
class AAshley_PlayerController;
class UAshley_SequencesDA;

UCLASS()
class ASHLEY_API AAshley_SequenceManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAshley_SequenceManager();

	UFUNCTION()
	void SkipSequence();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void PlaySequence(FGameplayTag SequenceIndex);

	UFUNCTION()
	void OnSequenceEnd(FGameplayTag SequenceIndex);

	// Bound to the sequence player's OnFinished: fires after the sequence has restored its state
	UFUNCTION()
	void OnSequencePlayerFinished();

	UFUNCTION()
	void OnPlayerEventTriggered(FGameplayTag EventName, FString EventParam);

	UPROPERTY()
	ULevelSequencePlayer* Player;

	UPROPERTY()
	FGameplayTag CurrentSequenceName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables")
	TObjectPtr<UAshley_SequencesDA> LevelSequences;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables")
	bool bIsSequenceOneTime = true;

	TMap<FGameplayTag, bool> hasSequenceBeenShown;

	AAshley_PlayerController* PlayerController;
};
