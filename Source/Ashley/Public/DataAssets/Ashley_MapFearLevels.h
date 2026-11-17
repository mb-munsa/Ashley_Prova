// Ashley

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"

#include "Managers/Ashley_PoltergeistManager.h"

#include "Ashley_MapFearLevels.generated.h"

/**
 * 
 */
UCLASS()
class ASHLEY_API UAshley_MapFearLevels : public UDataAsset
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Data")
	TArray<FFearLevel> FearLevels;
};
