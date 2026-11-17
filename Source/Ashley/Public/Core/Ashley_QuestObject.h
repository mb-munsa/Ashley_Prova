// Ashley

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interaction/Ashley_Interactable.h"

#include "Ashley_QuestObject.generated.h"

class AAshley_CharacterPlayer;
class UAshley_QuestDA;
class UAshley_PickupDA;

UCLASS()
class ASHLEY_API AAshley_QuestObject : public AAshley_Interactable
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAshley_QuestObject();

	virtual void Tick(float DeltaTime) override;

	virtual void Interact_Implementation() override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
	UAshley_QuestDA* QuestsData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
	UAshley_PickupDA* PickupsData;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DataMesh")
	//UStaticMeshComponent* Mesh;

	AAshley_CharacterPlayer* Character;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest", meta = (GetOptions = "GetQuestNames"))
	FName QuestName;

	UFUNCTION()
	TArray<FName> GetItemsNames() const;

	UFUNCTION()
	TArray<FName> GetQuestNames() const;

	UFUNCTION()
	TArray<FString> GetQuestSteps() const;
};
