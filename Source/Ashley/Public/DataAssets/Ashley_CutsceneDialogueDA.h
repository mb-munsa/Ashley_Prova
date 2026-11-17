// Ashley

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DataAssets/Ashley_GenericDialogue.h"
#include "Ashley_CutsceneDialogueDA.generated.h"

USTRUCT(BlueprintType)
struct FSequenceDialogues
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FDialogue> Dialogues;
};

UCLASS(BlueprintType)
class ASHLEY_API UAshley_CutsceneDialogueDA : public UDataAsset
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void PlayDialogue(UGameInstance* GameInstance, int SequenceIndex, int DialogueIndex);
	
protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<int, FSequenceDialogues> Sequences;
};
