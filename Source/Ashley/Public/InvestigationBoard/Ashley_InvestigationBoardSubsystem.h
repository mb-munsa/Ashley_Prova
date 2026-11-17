// Ashley

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Ashley_InvestigationBoardDefinition.h"

#include "Ashley_InvestigationBoardSubsystem.generated.h"

/**
 * Keeps the state of every investigation board across level loads and saves it to disk.
 */
UCLASS()
class ASHLEY_API UAshley_InvestigationBoardSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	FInvestigationBoardState& GetOrCreateState(FName BoardId);
	const FInvestigationBoardState* FindState(FName BoardId) const;

	UFUNCTION(BlueprintCallable)
	void SaveToDisk();

	UFUNCTION(BlueprintCallable)
	void LoadFromDisk();

	// Clears every board state and deletes the save slot (new game)
	UFUNCTION(BlueprintCallable)
	void ResetBoards();

	FName LastActiveBoardId;

protected:

	UPROPERTY()
	TMap<FName, FInvestigationBoardState> BoardStates;

	static const FString SaveSlotName;
};
