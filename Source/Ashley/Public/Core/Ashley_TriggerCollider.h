// Ashley

#pragma once

#include "CoreMinimal.h"
#include "NativeGameplayTags.h"
#include "GameFramework/Actor.h"

//#include "LevelSequence.h"
//#include "LevelSequencePlayer.h"
//#include "LevelSequenceActor.h"

#include "Ashley_TriggerCollider.generated.h"

// Delegate signature
//DECLARE_MULTICAST_DELEGATE_TwoParams(FOnScoreChangedSignature, int32 /* NewScore */, string /* aa */);
//DECLARE_MULTICAST_DELEGATE_OneParam(FOnHouseDoor, bool /*isPlayerInside*/);
//DECLARE_MULTICAST_DELEGATE(FOnHouseDoor);
//DECLARE_MULTICAST_DELEGATE_TwoParams(FOnEventTriggered, FGameplayTag /*Event Name*/, FString /*Event Paramter*/ );
//DECLARE_MULTICAST_DELEGATE(FOnHouseDoorVoid);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnTrigger);

class UBoxComponent;
class ULevelSequence;

UCLASS()
class ASHLEY_API AAshley_TriggerCollider : public AActor
{
	GENERATED_BODY()
	
public:
	AAshley_TriggerCollider();

	UFUNCTION(BlueprintCallable)
	void SetActive(bool bSetActive);

	//static FOnEventTriggered OnEventTriggered;

	UPROPERTY(BlueprintAssignable)
	FOnTrigger OnColliderTrigger;

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UFUNCTION()
	void OnComponentHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	UBoxComponent* TriggerVolume;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	bool bIsActive = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data|Event")
	bool bShouldTriggerEvent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data|Event", meta = (EditCondition = "bShouldTriggerEvent == true", EditConditionHides))
	FGameplayTag EventName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data|Event", meta = (EditCondition = "bShouldTriggerEvent == true", EditConditionHides))
	FString EventParam;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data|Event", meta = (EditCondition = "bShouldTriggerEvent == true", EditConditionHides))
	bool bIsEventOneShot;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data|Dialogue")
	bool bShouldShowDialogue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data|Dialogue", meta = (EditCondition = "bShouldShowDialogue == true", EditConditionHides))
	FString DialogueText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data|Dialogue", meta = (EditCondition = "bShouldShowDialogue == true", EditConditionHides))
	bool bIsDialogueOneTime;

	bool bHasAlreadyShownDialogue = false;
	bool bHasAlreadySentEvent = false;
};
