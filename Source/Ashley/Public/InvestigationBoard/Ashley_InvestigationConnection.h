// Ashley

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Ashley_InvestigationConnection.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FInvestigationConnection
{
	GENERATED_BODY()

	// Options come from UInvestigationBoardDefinition::GetPostItIds (the asset that owns this connection)
	UPROPERTY(EditAnywhere, meta = (GetOptions = "GetPostItIds"))
	FName A;

	UPROPERTY(EditAnywhere, meta = (GetOptions = "GetPostItIds"))
	FName B;

	FInvestigationConnection() {}

	FInvestigationConnection(FName NameA, FName NameB)
	{
		A = NameA;
		B = NameB;
	}

	bool operator==(const FInvestigationConnection& Other) const
	{
		return A == Other.A && B == Other.B 
			|| B == Other.A && A == Other.B;
	}

	// Order-independent, consistent with operator== (A-B == B-A)
	friend uint32 GetTypeHash(const FInvestigationConnection& Connection)
	{
		const bool bSwap = Connection.B.FastLess(Connection.A);

		return HashCombine(
			GetTypeHash(bSwap ? Connection.B : Connection.A),
			GetTypeHash(bSwap ? Connection.A : Connection.B)
		);
	}
};
