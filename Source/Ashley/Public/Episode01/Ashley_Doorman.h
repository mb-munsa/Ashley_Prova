// Ashley

#pragma once

#include "CoreMinimal.h"
#include "Characters/Ashley_NPC.h"
#include "GameplayTagContainer.h"
#include "Navigation/PathFollowingComponent.h"
#include "Perception/AIPerceptionTypes.h"
#include "Interaction/Ashley_Resettable.h"
#include "Ashley_Doorman.generated.h"

class AAshley_TriggerCollider;
class AAshley_Door;
class AAshley_ApartmentLevelManager;
class UAshley_GenericDialogue;
class UAshley_DialogueNode;
class UAIPerceptionComponent;
class UAIAsyncTaskBlueprintProxy;
class AAshley_CharacterPlayer;
class AAshley_PlayerController;

/**
 * Portiere del palazzo di Evan (port C++ di BP_Doorman)
 */
UCLASS()
class ASHLEY_API AAshley_Doorman : public AAshley_NPC, public IAshley_Resettable
{
	GENERATED_BODY()

public:
	AAshley_Doorman();

	virtual void Interact_Implementation() override;

	virtual void GoToCheckElectricalPanel_Implementation() override;

	UFUNCTION(BlueprintCallable)
	void GoCheckIntercom();

	// Blocca l'input del giocatore quando viene scoperto
	UFUNCTION(BlueprintCallable)
	void TriggerPlayerDeath();

	virtual void ResetToInitialState_Implementation() override;

	UFUNCTION(BlueprintPure)
	bool IsDoormanWatching() const { return bIsDoormanWatching; }

protected:
	virtual void BeginPlay() override;

	virtual void EndDialogue() override;

	// Dialogo intro o intro breve, in base a bIntroDialoguePlayed
	void PlayIntroDialogue();

	UFUNCTION()
	void OnBoxDialogueHit();

	UFUNCTION()
	void OnPlayerEvent(FGameplayTag EventName, FString EventParameter);

	UFUNCTION()
	void OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);

	// --- Sequenza quadro elettrico ---
	void OnElectricalPanelDelayElapsed();

	UFUNCTION()
	void OnReachedCourtyardDoor(EPathFollowingResult::Type MovementResult);

	// --- Sequenza citofono ---
	void OnIntercomDelayElapsed();

	UFUNCTION()
	void OnReachedIntercom(EPathFollowingResult::Type MovementResult);

	// --- Fine cutscene ---
	void OnCutsceneDialogueDelayElapsed();

	// Equivalente del nodo AI MoveTo
	UAIAsyncTaskBlueprintProxy* MoveToActor(AActor* Target, float AcceptanceRadius, bool bStopOnOverlap);

	// Componente AIPerception configurato nel BP (trovato a runtime)
	UPROPERTY(Transient)
	TObjectPtr<UAIPerceptionComponent> PerceptionComponent;

	// --- Riferimenti di livello ---
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data|Doorman|References")
	TObjectPtr<AAshley_ApartmentLevelManager> LevelManager;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data|Doorman|References")
	TObjectPtr<AAshley_TriggerCollider> Trigger1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data|Doorman|References")
	TObjectPtr<AAshley_Door> ConciergeDoor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data|Doorman|References")
	TObjectPtr<AAshley_Door> CourtyardDoor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data|Doorman|References")
	TObjectPtr<AActor> ElectricalPanelDestination;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data|Doorman|References")
	TObjectPtr<AActor> IntercomDestination;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data|Doorman|References")
	TObjectPtr<AActor> SpawnTransform;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data|Doorman|References")
	TObjectPtr<AActor> CharacterSpawnTransform;

	// --- Dialoghi ---
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data|Doorman|Dialogue")
	TObjectPtr<UAshley_DialogueNode> IntroDialogue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data|Doorman|Dialogue")
	TObjectPtr<UAshley_DialogueNode> IntroShortDialogue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data|Doorman|Dialogue")
	TObjectPtr<UAshley_GenericDialogue> FinalDialogue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data|Doorman|Dialogue")
	FGameplayTag DoormanCutsceneName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data|Doorman|Dialogue")
	FName PhotoItemID = TEXT("DoormanPhoto");

	// --- Tempi / movimento ---
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data|Doorman|Movement")
	float ElectricalPanelDelay = 2.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data|Doorman|Movement")
	float IntercomDelay = 2.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data|Doorman|Movement")
	float CutsceneDialogueDelay = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data|Doorman|Movement")
	float DefaultAcceptanceRadius = 5.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data|Doorman|Movement")
	float IntercomAcceptanceRadius = 10.f;

	// --- Stato ---
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data|Doorman|State")
	bool bIsDoormanWatching = true;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Data|Doorman|State")
	bool bIntroDialoguePlayed = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Data|Doorman|State")
	bool bPlayerSpotted = false;

private:
	AAshley_CharacterPlayer* GetPlayerCharacter();
	AAshley_PlayerController* GetPlayerController();

	UPROPERTY(Transient)
	TObjectPtr<AAshley_CharacterPlayer> CachedPlayerCharacter;

	UPROPERTY(Transient)
	TObjectPtr<AAshley_PlayerController> CachedPlayerController;

	FTimerHandle ElectricalPanelTimer;
	FTimerHandle IntercomTimer;
	FTimerHandle CutsceneDialogueTimer;
};
