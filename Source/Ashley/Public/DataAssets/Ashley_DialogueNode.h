// Ashley

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DataAssets/Ashley_GenericDialogue.h"
#include "Ashley_DialogueNode.generated.h"

class UAshley_ChoiceMenu;
class UAshley_DialogueCondition;

USTRUCT(BlueprintType)
struct FDialogueChoice
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int ChoiceIndex = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText DialogueChoice;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAshley_DialogueNode* FollowingNode = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsOneShot = true;

	// requisiti per mostrare questa scelta
	UPROPERTY(EditDefaultsOnly, Instanced)
	TArray<TObjectPtr<UAshley_DialogueCondition>> Conditions;
};

/**
 * 
 */
UCLASS(BlueprintType)
class ASHLEY_API UAshley_DialogueNode : public UDataAsset
{
	GENERATED_BODY()

public:

#if WITH_EDITOR
	void PostInitProperties() override;
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

	UPROPERTY(VisibleDefaultsOnly)
	FGuid NodeID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsChoiceMenu;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data", meta = (EditCondition = "bIsChoiceMenu == true", EditConditionHides))
	TArray<FDialogueChoice> DialogueChoices;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data", meta = (EditCondition = "bIsChoiceMenu == false", EditConditionHides))
	TObjectPtr<UAshley_DialogueNode> FollowingNode;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data", meta = (EditCondition = "bIsChoiceMenu == false", EditConditionHides))
	FDialogue Dialogue;

	UPROPERTY()
	UAshley_ChoiceMenu* ChoiceMenu;
	
};
