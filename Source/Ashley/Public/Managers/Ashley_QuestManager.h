// Ashley

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DataAssets/Ashley_QuestDA.h"

#include "Ashley_QuestManager.generated.h"

USTRUCT(BlueprintType)
struct FActiveQuest
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere)
	FName QuestName;
	UPROPERTY(VisibleAnywhere)
	FString CurrentQuestDesc;
	UPROPERTY(VisibleAnywhere)
	FText CurrentQuestDialogue;
	UPROPERTY(VisibleAnywhere)
	int QuestStep = 0;
};

UCLASS()
class ASHLEY_API AAshley_QuestManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAshley_QuestManager();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAshley_QuestDA* QuestConfiguration;

	UFUNCTION()
	const TArray<FActiveQuest>& GetActiveQuests();

	UFUNCTION()
	void AdvanceQuest(const FName& QuestName, int Step = -1);

	void AdvanceQuest(const FName& QuestName, FString StepName);

	UFUNCTION()
	void CompleteQuest(const FName& QuestName);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	TArray<FActiveQuest> ActiveQuests {};

};
