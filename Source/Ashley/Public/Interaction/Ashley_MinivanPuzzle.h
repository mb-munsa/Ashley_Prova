// Ashley

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include <Interaction/Ashley_ClickableInterface.h>

#include "Ashley_MinivanPuzzle.generated.h"

// Called when the user clicked to start the puzzle
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPuzzleClicked, AAshley_MinivanPuzzle*, Puzzle);

// Called when the puzzle has been completed
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPuzzleCompleted, AAshley_MinivanPuzzle*, Puzzle);

class UAshley_PickupDA;

UCLASS()
class ASHLEY_API AAshley_MinivanPuzzle : public AActor, public IAshley_ClickableInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAshley_MinivanPuzzle();

	virtual void Click_Implementation() override;

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnPuzzleClicked OnPuzzleClicked;

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnPuzzleCompleted OnPuzzleCompleted;

	UFUNCTION(BlueprintCallable)
	void CompletePuzzle();

	UFUNCTION(BlueprintCallable)
	void StartPuzzleEvent();

	UFUNCTION(BlueprintCallable)
	void ExitPuzzleEvent();

	UFUNCTION(BlueprintNativeEvent)
	void StartPuzzle();

	UFUNCTION(BlueprintNativeEvent)
	void ExitPuzzle();

	FName GetPickupName() const { return PickupName; }

	bool IsExitBlocked() const { return bExitBlocked; }

	void SetExitBlocked(bool bShouldBlockExit) { bExitBlocked = bShouldBlockExit; }

protected:
	//// Called when the game starts or when spawned
	//virtual void BeginPlay() override;

	UFUNCTION()
	TArray<FName> GetValidPickupIDs() const;

	UFUNCTION()
	TArray<FName> GetValidHintIDs() const;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data|Interactable")
	UStaticMeshComponent* Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data|Interactable")
	bool bIsPuzzleCompleted;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data|Inventory")
	TSoftObjectPtr<UAshley_PickupDA> PickupDataAsset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data|Inventory", meta = (GetOptions = "GetValidPickupIDs"))
	FName PickupName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data|Inventory")
	bool bGivesHint = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data|Inventory", meta = (EditCondition = bGivesHint, EditConditionHides))
	TSoftObjectPtr<UAshley_PickupDA> HintsDataAsset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data|Inventory", meta = (EditCondition = bGivesHint, EditConditionHides), meta = (GetOptions = "GetValidHintIDs"))
	TArray<FName> HintsNames;

	// True mentre il puzzle è risolto ma non ancora completato: uscire ora perderebbe la vittoria
	bool bExitBlocked = false;
};
