// Ashley


#include "InvestigationBoard/Ashley_InvestigationBoardSubsystem.h"
#include "InvestigationBoard/Ashley_InvestigationSaveGame.h"

#include "Kismet/GameplayStatics.h"

const FString UAshley_InvestigationBoardSubsystem::SaveSlotName = TEXT("InvestigationBoard");

void UAshley_InvestigationBoardSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	// TODO: saving is disabled for now, every play starts with empty boards.
	// To re-enable it, replace ResetBoards() with LoadFromDisk()
	ResetBoards();
}

FInvestigationBoardState& UAshley_InvestigationBoardSubsystem::GetOrCreateState(FName BoardId)
{
	return BoardStates.FindOrAdd(BoardId);
}

const FInvestigationBoardState* UAshley_InvestigationBoardSubsystem::FindState(FName BoardId) const
{
	return BoardStates.Find(BoardId);
}

void UAshley_InvestigationBoardSubsystem::SaveToDisk()
{
	UAshley_InvestigationSaveGame* SaveGame = Cast<UAshley_InvestigationSaveGame>(
		UGameplayStatics::CreateSaveGameObject(UAshley_InvestigationSaveGame::StaticClass()));

	if (!SaveGame)
		return;

	SaveGame->BoardStates = BoardStates;
	SaveGame->LastActiveBoardId = LastActiveBoardId;

	UGameplayStatics::SaveGameToSlot(SaveGame, SaveSlotName, 0);
}

void UAshley_InvestigationBoardSubsystem::LoadFromDisk()
{
	if (!UGameplayStatics::DoesSaveGameExist(SaveSlotName, 0))
		return;

	const UAshley_InvestigationSaveGame* SaveGame = Cast<UAshley_InvestigationSaveGame>(
		UGameplayStatics::LoadGameFromSlot(SaveSlotName, 0));

	if (!SaveGame)
		return;

	BoardStates = SaveGame->BoardStates;
	LastActiveBoardId = SaveGame->LastActiveBoardId;
}

void UAshley_InvestigationBoardSubsystem::ResetBoards()
{
	BoardStates.Empty();
	LastActiveBoardId = NAME_None;

	if (UGameplayStatics::DoesSaveGameExist(SaveSlotName, 0))
		UGameplayStatics::DeleteGameInSlot(SaveSlotName, 0);
}
