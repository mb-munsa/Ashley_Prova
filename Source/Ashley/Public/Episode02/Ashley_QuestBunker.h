// Ashley

#pragma once

#include "CoreMinimal.h"
#include "Core/Ashley_QuestObject.h"
#include "Ashley_QuestBunker.generated.h"

class UAshley_PickupDA;

/**
 * 
 */
UCLASS()
class ASHLEY_API AAshley_QuestBunker : public AAshley_QuestObject
{
	GENERATED_BODY()

public:

	virtual void Interact_Implementation() override;

	virtual FString GetInteractMessage() const override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest", meta = (GetOptions = "GetItemsNames"))
	FName ShelterKeysID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest", meta = (GetOptions = "GetItemsNames"))
	FName HammerID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest", meta = (GetOptions = "GetItemsNames"))
	FName NailsID;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintImplementableEvent)
	void PlayDoorOpenAnimation();

	enum ShelterState
	{
		InitialState,
		NeedHammerNails,
		NeedHammer,
		NeedNails,
		RepairedDoor,
		FinalState
	};

	ShelterState CurrentState = InitialState;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "QuestSteps", meta = (GetOptions = "GetQuestSteps"))
	FString FirstStep;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "QuestSteps", meta = (GetOptions = "GetQuestSteps"))
	FString NeedHammerNailsStep;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "QuestSteps", meta = (GetOptions = "GetQuestSteps"))
	FString NeedOneMoreItemStep;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "QuestSteps", meta = (GetOptions = "GetQuestSteps"))
	FString RepairedDoorStep;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "QuestSteps", meta = (GetOptions = "GetQuestSteps"))
	FString FinalStep;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
	TArray<FDialogueChoice> Choices;
};
