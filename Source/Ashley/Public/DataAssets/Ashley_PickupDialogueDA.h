// Ashley

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"

#include "Ashley_PickupDialogueDA.generated.h"

class UAshley_InventoryComponent;
class UAshley_PickupDA;

USTRUCT(BlueprintType)
struct FPickupDialogueChoice
{
	GENERATED_BODY()

public:
	// TODO: valutare se generalizzare queste condizioni
	// Conditions
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FName> ItemsRequired;

	// Dialogue triggered
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Dialogue;
};

USTRUCT(BlueprintType)
struct FPickupDialogues
{
    GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FPickupDialogueChoice> DialogueChoices;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText GenericDialogue;
};

UCLASS()
class ASHLEY_API UAshley_PickupDialogueDA : public UDataAsset
{
	GENERATED_BODY()

protected:

	UFUNCTION()
	TArray<FName> GetItemNames() const;

public:

	UFUNCTION()
	FText GetDialogueForPickup(FName ItemID, UAshley_InventoryComponent* Inventory);

	FText GetGenericDialogueForPickup(FName ItemID);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UAshley_PickupDA> ItemData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (GetOptions = "GetItemNames"))
	TMap<FName, FPickupDialogues> Dialogues;
};
