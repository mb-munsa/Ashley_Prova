// Ashley

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Ashley_InvestigationConnection.h"
#include "Ashley_InvestigationPostIt.generated.h"

USTRUCT(BlueprintType)
struct FInvestigationPostIt
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	FName Id;

	UPROPERTY(EditAnywhere)
	FText Title;

	UPROPERTY(EditAnywhere, meta = (MultiLine = true))
	FText Description;

	// Options come from UInvestigationBoardDefinition::GetValidIDs (the asset that owns this post-it)
	UPROPERTY(EditAnywhere, meta = (GetOptions = "GetValidIDs"))
	FName RequiredInventoryItem;

	UPROPERTY(EditAnywhere)
	bool bRequireDeduction = false;

	UPROPERTY(EditAnywhere)
	FVector2D DefaultPosition = FVector2D::ZeroVector;

	UPROPERTY(EditAnywhere)
	UTexture2D* Image = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visibility",
		meta = (EditCondition = "bRequireDeduction", ClampMin = "1"))
	int32 RequiredConnectionCount = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visibility",
		meta = (EditCondition = "bRequireDeduction"))
	TArray<FInvestigationConnection> RequiredConnections;
};
