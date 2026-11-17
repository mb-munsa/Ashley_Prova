// Ashley

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"

#include "Ashley_QuestDA.generated.h"

USTRUCT(BlueprintType)
struct FQuestStep
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString StepTitle;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FText StepDialogue;
};


USTRUCT(BlueprintType)
struct FQuest
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FName QuestID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FQuestStep> QuestSteps;
};


UCLASS()
class ASHLEY_API UAshley_QuestDA : public UDataAsset
{
	GENERATED_BODY()

public:

    UFUNCTION()
    const FString& GetStepTitleFromQuest(FName QuestName, int QuestStep);

    UFUNCTION()
    const FText& GetDialogueFromQuest(FName QuestName, int QuestStep);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (TitleProperty = QuestID))
	TArray<FQuest> Quests;

};
