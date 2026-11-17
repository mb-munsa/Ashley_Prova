// Ashley

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "Ashley_InvestigationBoardDefinition.h"

#include "Ashley_InvestigationSaveGame.generated.h"

UCLASS()
class ASHLEY_API UAshley_InvestigationSaveGame : public USaveGame
{
	GENERATED_BODY()

public:

	UPROPERTY()
	TMap<FName, FInvestigationBoardState> BoardStates;

	UPROPERTY()
	FName LastActiveBoardId;
};
