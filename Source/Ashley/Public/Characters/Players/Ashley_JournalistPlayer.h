// Ashley

#pragma once

#include "CoreMinimal.h"
#include "Characters/Players/Ashley_CharacterPlayer.h"
#include "Ashley_JournalistPlayer.generated.h"

/**
 * 
 */
UCLASS()
class ASHLEY_API AAshley_JournalistPlayer : public AAshley_CharacterPlayer
{
	GENERATED_BODY()
public:

	virtual void PossessedBy(AController* NewController) override;

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data|Inventory")
	TSubclassOf<UAshley_InventoryUI> DiaryBP;
	
};
