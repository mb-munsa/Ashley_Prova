// Ashley

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Ashley_Resettable.generated.h"

UINTERFACE(MinimalAPI, Blueprintable)
class UAshley_Resettable : public UInterface
{
	GENERATED_BODY()
};

/**
 * Versione C++ di BPI_Resettable: attori che tornano allo stato iniziale al reset del livello
 */
class ASHLEY_API IAshley_Resettable
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Reset")
	void ResetToInitialState();

	// Resetta un attore che implementa IAshley_Resettable oppure, in fallback, il vecchio BPI_Resettable
	static void ResetActor(AActor* Actor);
};
