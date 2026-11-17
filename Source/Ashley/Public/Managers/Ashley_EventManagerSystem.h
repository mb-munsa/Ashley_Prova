// Ashley

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "NativeGameplayTags.h"

#include "Ashley_EventManagerSystem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnEventTriggered, FGameplayTag, EventName, FString, EventParameter);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnItemSelected, FName, ItemName);


/**
 * 
 */
UCLASS()
class ASHLEY_API UAshley_EventManagerSystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintAssignable)
	FOnEventTriggered OnEventTriggered;
	UPROPERTY(BlueprintAssignable)
	FOnItemSelected OnItemSelected;

	UFUNCTION(BlueprintCallable)
	void BroadcastEventTriggered(FGameplayTag Event, FString EventParam);

	void BroadcastItemSelected(FName ItemName);
	
};
