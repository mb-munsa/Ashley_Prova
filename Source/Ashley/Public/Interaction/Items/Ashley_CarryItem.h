// Ashley

#pragma once

#include "CoreMinimal.h"
#include "Interaction/Ashley_ActorInteract.h"
#include "Ashley_CarryItem.generated.h"

class AAshley_CharacterPlayer;
class UAshley_PickupDA;
class UInputMappingContext;

/**
 * 
 */
UCLASS()
class ASHLEY_API AAshley_CarryItem : public AAshley_ActorInteract
{
	GENERATED_BODY()

public:
	AAshley_CarryItem();

	virtual void Interact_Implementation() override;

	UFUNCTION()
	virtual void Drop();

	UFUNCTION(BlueprintCallable)
	virtual FName GetID();

	UFUNCTION(BlueprintCallable)
	virtual bool IsDestroyOnUse();

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data|Item")
	TObjectPtr<UStaticMeshComponent> MeshBase;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data|Item", meta = (GetOptions = "GetValidIDs"))
	FName Name_ID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data|Item")
	TSoftObjectPtr<UAshley_PickupDA> PickupDataAsset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data|Item")
	bool bIsDestroyOnUse = true;

	UFUNCTION()
	TArray<FName> GetValidIDs() const;

	bool bIsBeingCarried;
	AAshley_CharacterPlayer* CharacterRef;
	ECollisionEnabled::Type PreviousCollisionPreset;
};
