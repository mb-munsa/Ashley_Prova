// Ashley

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "NativeGameplayTags.h"

#include "Ashley_GameStateManager.generated.h"

/**
 * 
 */
UCLASS()
class ASHLEY_API UAshley_GameStateManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:

	UFUNCTION(BlueprintCallable)
	void SetTag(FGameplayTag Tag, bool bEnabled = true);

	UFUNCTION(BlueprintCallable)
	bool GetTagValue(FGameplayTag Tag);

protected:

	TMap<FGameplayTag, bool> GameStateTags;

};
