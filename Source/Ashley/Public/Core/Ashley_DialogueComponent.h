// Ashley

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DataAssets/Ashley_DialogueNode.h"
#include "Ashley_DialogueComponent.generated.h"

class UAshley_DialogueNode;
class UAshley_ChoiceMenu;
class UAshley_WidgetSystem;

USTRUCT()
struct FDialogueChoiceKey
{
	GENERATED_BODY()

	UPROPERTY()
	FGuid NodeID;

	UPROPERTY()
	int32 ChoiceIndex = 0;

	bool operator==(const FDialogueChoiceKey& Other) const
	{
		return NodeID == Other.NodeID && ChoiceIndex == Other.ChoiceIndex;
	}
};

inline uint32 GetTypeHash(const FDialogueChoiceKey& Key)
{
	return HashCombine(GetTypeHash(Key.NodeID), GetTypeHash(Key.ChoiceIndex));
}


// evento di fine dialogo
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDialogueEnded);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnChoiceVisited, int, ChoiceIndex);


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ASHLEY_API UAshley_DialogueComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UAshley_DialogueComponent();

	virtual void OnRegister() override;

	UPROPERTY(BlueprintAssignable)
	FOnDialogueEnded OnDialogueEnded;

	UPROPERTY(BlueprintAssignable)
	FOnChoiceVisited OnChoiceVisited;

	UFUNCTION(BlueprintCallable)
	bool IsChoiceAvailable(FDialogueChoice Choice);

	UFUNCTION(BlueprintCallable)
	void Play(UAshley_DialogueNode* DialogueNode);

	UFUNCTION(BlueprintCallable)
	void PlayDialogueLine(int LineIndex);

	UFUNCTION(BlueprintCallable)
	void SkipToNextLine();

	UFUNCTION(BlueprintCallable)
	void OnChoiceSelected(int ChoiceIndex);

protected:

	UPROPERTY()
	bool bIsDialogueOngoing;

	UPROPERTY()
	UAshley_ChoiceMenu* ChoiceMenu;

	UPROPERTY()
	UAudioComponent* MyAudioComponent; // TODO: get

	UPROPERTY()
	TSet<FDialogueChoiceKey> ChoicesVisited;

	UPROPERTY()
	int CurrentLineIndex = 0;

	UPROPERTY()
	UAshley_DialogueNode* CurrentDialogueNode;

	UPROPERTY()
	FTimerHandle TimerHandle_NextLine;

	UAshley_WidgetSystem* WidgetManager;
};
