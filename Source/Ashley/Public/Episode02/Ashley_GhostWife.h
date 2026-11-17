// Ashley

#pragma once

#include "CoreMinimal.h"
#include "Interaction/Ashley_Interactable.h"
#include "Ashley_GhostWife.generated.h"

class AAshley_GhostSpawner;

/**
 * 
 */
UCLASS()
class ASHLEY_API AAshley_GhostWife : public AAshley_Interactable
{
	GENERATED_BODY()

public:

	void SetGhostSpawner(TObjectPtr<AAshley_GhostSpawner> GhostController);

protected:

	UFUNCTION(BlueprintCallable)
	void CompleteQuest();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Data")
	TObjectPtr<AAshley_GhostSpawner> GhostSpawner;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	FString FinalSequenceIndex = "5";
};
