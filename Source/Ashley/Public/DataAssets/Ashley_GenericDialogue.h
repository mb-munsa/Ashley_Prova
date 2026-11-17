// Ashley

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Tags_Enums_Structs/Ashley_GameplayTags.h"
#include "Ashley_GenericDialogue.generated.h"

USTRUCT(BlueprintType)
struct FDialogueLine
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Dialogue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USoundBase* DialogueVoiceover = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DialogueDuration = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTag CharacterSpeaking = AshleyGameplayTags::Ashley_Voice_Narrator;
};

USTRUCT(BlueprintType)
struct FDialogue
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FDialogueLine> DialogueLines;
};

DECLARE_DYNAMIC_DELEGATE(FOnGenericDialogueEnded);

/**
 *  Contiene una lista di dialoghi, che vengono riprodotti in sequenza
 */
UCLASS(BlueprintType)
class ASHLEY_API UAshley_GenericDialogue : public UDataAsset
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable)
	void PlayDialogue(UGameInstance* GameInstance, int DialogueIndex = -1);

	// OnDialogueEnded viene chiamato quando l'ultima battuta viene nascosta (timer o skip del giocatore)
	void PlayDialogue(UGameInstance* GameInstance, int DialogueIndex, FSimpleDelegate OnDialogueEnded);

	// Versione Blueprint di PlayDialogue con callback
	UFUNCTION(BlueprintCallable, meta = (AutoCreateRefTerm = "OnDialogueEnded"))
	void PlayDialogueWithCallback(UGameInstance* GameInstance, int DialogueIndex, const FOnGenericDialogueEnded& OnDialogueEnded);

	UFUNCTION(BlueprintCallable)
	void PlayGenericDialogue(UGameInstance* GameInstance, int DialogueIndex = 0);

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FDialogue> Dialogues;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<UAshley_GenericDialogue*> GenericDialogues;

	UPROPERTY()
	int LastDialogueIndex;
};
