// Ashley

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Ashley_PoltergeistDA.generated.h"


UENUM(BlueprintType)
enum class EPoltergeistActivationType : uint8
{
	ALWAYS_ACTIVE UMETA(DisplayName = "Always active"),
	ACTIVATE_BY_PROXIMITY_ALWAYS UMETA(DisplayName = "Activated by proximity always"),
	ACTIVATE_BY_PROXIMITY_RANDOM UMETA(DisplayName = "Activated by proximity random")
};

UENUM(BlueprintType)
enum class EPoltergeistFrequency : uint8
{
	ONE_SHOT UMETA(DisplayName = "One shot"),
	ONE_SHOT_EVERYONE UMETA(DisplayName = "One shot everyone"),
	REPEATED UMETA(DisplayName = "Repeated")
};

/**
 * 
 */
UCLASS()
class ASHLEY_API UAshley_PoltergeistDA : public UDataAsset
{
	GENERATED_BODY()

public:

	UFUNCTION()
	uint8 GetActivationLevel() const;

	UFUNCTION()
	EPoltergeistFrequency GetFrequency() const;

	UFUNCTION()
	EPoltergeistActivationType GetActivationType() const;

	UFUNCTION()
	double GetCooldown() const;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data|Type")
	EPoltergeistActivationType ActivationType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data|Type")
	EPoltergeistFrequency Frequency;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data|Type")
	uint8 ActivationLevel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data|Type")
	double Cooldown;
	
};
