// Ashley

#pragma once

#include "CoreMinimal.h"
#include "Core/Ashley_QuestObject.h"
#include "Ashley_QuestCar.generated.h"

class UAshley_PickupDA;
class UAshley_QuestDA;

/**
 * 
 */
UCLASS()
class ASHLEY_API AAshley_QuestCar : public AAshley_QuestObject
{
	GENERATED_BODY()

public:

	virtual void Interact_Implementation() override;

	virtual FString GetInteractMessage() const override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest", meta = (GetOptions = "GetItemsNames"))
	FName CarKeyID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest", meta = (GetOptions = "GetItemsNames"))
	FName PetrolTankID;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	enum CarStates
	{
		InitialState,
		NeedPetrol,
		GettingReady,
		FinalState
	};

	CarStates CurrentState = InitialState;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest", meta = (GetOptions = "GetQuestSteps"))
	FString NeedKeyDialogue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest", meta = (GetOptions = "GetQuestSteps"))
	FString NeedPetrolDialogue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest", meta = (GetOptions = "GetQuestSteps"))
	FString FinalDialogue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
	TArray<FDialogueChoice> Choices;
};
