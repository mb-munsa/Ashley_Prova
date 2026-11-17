// Ashley

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "NativeGameplayTags.h"

#include "Ashley_SequencesDA.generated.h"

class ULevelSequence;

USTRUCT(BlueprintType)
struct FSequence
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTag SequenceName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ULevelSequence* SequenceObject = nullptr;

	bool operator==(const FGameplayTag& OtherTag) const
	{
		return SequenceName == OtherTag;
	}
};

/**
 * 
 */
UCLASS()
class ASHLEY_API UAshley_SequencesDA : public UDataAsset
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables")
	TArray<FSequence> Sequences;
	
};
