// Ashley

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Ashley_InvestigationConnection.h"
#include "Ashley_InvestigationPostIt.h"

#include "Ashley_InvestigationBoardDefinition.generated.h"

class UAshley_PickupDA;

USTRUCT()
struct FInvestigationBoardState
{
	GENERATED_BODY()

public:

	// False until the board has been computed the first time
	UPROPERTY()
	bool bInitialized = false;

	// Post-its currently revealed to the player.
	UPROPERTY()
	TSet<FName> VisiblePostIts;

	// Connections the player has established.
	UPROPERTY()
	TSet<FInvestigationConnection> Connections;

	// Deductions the player has discovered.
	UPROPERTY()
	TSet<FName> DiscoveredDeductions;
};

UCLASS()
class ASHLEY_API UInvestigationBoardDefinition : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:

	// Key used to save the board state. If None, the asset name is used
	UPROPERTY(EditAnywhere)
	FName BoardId;

	UPROPERTY(EditAnywhere)
	FText Title;

	// If false, the board appears only once RequiredInventoryItem is in the inventory
	UPROPERTY(EditAnywhere, Category = "Unlock")
	bool bUnlockedByDefault = true;

	UPROPERTY(EditAnywhere, Category = "Unlock", meta = (EditCondition = "!bUnlockedByDefault", GetOptions = "GetValidIDs"))
	FName RequiredInventoryItem;

	// Items selectable as RequiredInventoryItem (board and post-its)
	UPROPERTY(EditAnywhere, Category = "Data|Inventory")
	TSoftObjectPtr<UAshley_PickupDA> PickupDataAsset;

	// None + every ItemID in PickupDataAsset
	UFUNCTION()
	TArray<FName> GetValidIDs() const;

	UPROPERTY(EditAnywhere)
	TArray<FInvestigationPostIt> PostIts;

	UPROPERTY(EditAnywhere)
	TArray<FInvestigationConnection> Connections;

	// Every post-it Id of this board, used by the connection dropdowns
	UFUNCTION()
	TArray<FName> GetPostItIds() const;

#if WITH_EDITOR
	// Checks that connections use existing post-its and that required connections are in Connections
	virtual EDataValidationResult IsDataValid(FDataValidationContext& Context) const override;
#endif

	FName GetBoardId() const
	{
		return BoardId.IsNone() ? GetFName() : BoardId;
	}
};
