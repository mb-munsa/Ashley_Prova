// Ashley

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DataAssets/Ashley_PickupDialogueDA.h"
#include "Ashley_LevelDialogueManager.generated.h"

class AAshley_CharacterPlayer;
class AAshley_PlayerController;

UCLASS()
class ASHLEY_API AAshley_LevelDialogueManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAshley_LevelDialogueManager();

	UFUNCTION()
	void OnPlayerItemPickup(FName ItemID);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	AAshley_CharacterPlayer* Character;
	AAshley_PlayerController* Player;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAshley_PickupDialogueDA* PickupDialogueDA;

	TSet<FName> ItemsAlreadySeen;
};
