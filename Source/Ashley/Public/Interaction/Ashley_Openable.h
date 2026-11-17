// Ashley

#pragma once

#include "CoreMinimal.h"
#include "Interaction/Ashley_ActorInteract.h"
#include "DataAssets/Ashley_CutsceneDialogueDA.h"
#include "NativeGameplayTags.h"

#include "Ashley_Openable.generated.h"

UENUM(BlueprintType)
enum class EState : uint8
{
	OPEN UMETA(DisplayName = "Open"),
	CLOSED UMETA(DisplayName = "Closed")
};

UENUM(BlueprintType)
enum class ELockType : uint8
{
	UNLOCKED UMETA(DisplayName = "Unlocked"),
	LOCKED_KEY UMETA(DisplayName = "LockedWithKey"),
	LOCKED UMETA(DisplayName = "Locked")
};

struct FInventoryInfo;
class UAshley_PickupDA;
UCLASS()
class ASHLEY_API AAshley_Openable : public AAshley_ActorInteract
{
	GENERATED_BODY()
	
public:
	AAshley_Openable();

	virtual void Interact_Implementation() override;
	virtual FString GetInteractMessage() const override;

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent);
#endif

	UFUNCTION(BlueprintCallable)
	virtual void SwitchToOpen();

	UFUNCTION(BlueprintCallable)
	virtual void SwitchToClosed();

	UFUNCTION(BlueprintCallable)
	virtual void ResetState();

protected:

	virtual void BeginPlay() override;

	virtual void GivePickup();

	virtual void NotifyActorEndOverlap(AActor* OtherActor) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data|Mesh")
	TObjectPtr<UStaticMeshComponent> MeshBase;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data|Mesh")
	TObjectPtr<UStaticMeshComponent> MeshOpenable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data|State")
	EState InitialState;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data|State")
	ELockType LockType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data|State", meta = (EditCondition = " State == EState::CLOSEDKEY", EditConditionHides), meta = (GetOptions = "GetValidIDs"))
	FName Key;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data|Pickup")
	bool bIsThereAPickup;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data|Pickup", meta = (GetOptions = "GetValidIDs", EditCondition = "bIsThereAPickup == true", EditConditionHides))
	FName Name_ID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data|Pickup", meta = (EditCondition = "bIsThereAPickup == true", EditConditionHides))
	int StackSize = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data|State")
	bool bShouldAutoclose;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data|State")
	bool bCanBeClosed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data|State")
	bool bShouldTriggerEvent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data|State", meta = (EditCondition = "bShouldTriggerEvent == true", EditConditionHides))
	FGameplayTag EventName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data|State", meta = (EditCondition = " bShouldTriggerEvent == true", EditConditionHides))
	FString EventParam;

	UFUNCTION()
	TArray<FName> GetValidIDs() const;

	UPROPERTY(EditAnywhere, Category = "Data|Pickup", meta = (EditCondition = "bIsThereAPickup == true", EditConditionHides))
	TSoftObjectPtr<UAshley_PickupDA> PickupDataAsset;

	UFUNCTION(BlueprintImplementableEvent, Category = "Door")
	void OpenCloseDoor(bool bOpen);

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Data|Rotation")
	float InitialOpenableRotation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data|Dialogue")
	UAshley_GenericDialogue* ClosedDoorDialogue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data|Dialogue")
	UAshley_GenericDialogue* OpenDoorDialogue;

	UPROPERTY(BlueprintReadWrite, Category = "Data|Dialogue")
	bool bFirstOpenDone = false;

	UPROPERTY(BlueprintReadWrite)
	EState CurrentState;

private:
	UPROPERTY(VisibleAnywhere, Category = "Data|Pickup")
	FInventoryInfo Pickup;
};
