// Ashley

#pragma once

#include "CoreMinimal.h"
#include "NativeGameplayTags.h"
#include "Interaction/Ashley_ActorInteract.h"

#include "Ashley_Torch.generated.h"

class USpotLightComponent;
class UPointLightComponent;
class UAudioComponent;
class AAshley_CharacterPlayer;

/**
 * 
 */
UCLASS()
class ASHLEY_API AAshley_Torch : public AAshley_ActorInteract
{
	GENERATED_BODY()

public:
	AAshley_Torch();

	virtual void Interact_Implementation() override;

	virtual void TurnOn();

	virtual void TurnOff();

	UFUNCTION()
	void EquipTorch();

protected:

	UFUNCTION()
	void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	TObjectPtr<UStaticMeshComponent> MeshBase;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	TObjectPtr<USpotLightComponent> Light;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Data")
	TObjectPtr<UPointLightComponent> LightPoint;

	// Audio component
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	UAudioComponent* AudioComponent;

	AAshley_CharacterPlayer* Character;

	bool bIsEquipped = false;

	bool bIsOn = false;
};
