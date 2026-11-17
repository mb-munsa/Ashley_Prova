// Ashley

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NativeGameplayTags.h"
#include "Ashley_ApartmentLevelManager.generated.h"

class UBoxComponent;
class AAshley_Doorman;
class AAshley_Torch;
class UAshley_PickupDA;
class UAshley_GenericDialogue;
class AAshley_Interactable;
class AAshley_TriggerCollider;

UCLASS()
class ASHLEY_API AAshley_ApartmentLevelManager : public AActor
{
	GENERATED_BODY()
	
public:	
	AAshley_ApartmentLevelManager();

	UFUNCTION(BlueprintCallable)
	void SetFirstDialogueDone();

	UFUNCTION(BlueprintImplementableEvent)
	void ActivateCourtyard();

	UFUNCTION(BlueprintCallable)
	void ActivateElectricalPanel();

	UFUNCTION()
	void OnPlayerEventTriggered(FGameplayTag EventName, FString EventParam);

	UFUNCTION()
	void OnPlayerItemPickup(FName ItemID);

	// Il giocatore e' stato scoperto: resetto doorman, player, citofono e quadro elettrico
	UFUNCTION(BlueprintCallable)
	void ResetLevel();

	UFUNCTION(BlueprintCallable)
	void ActivateIntercom();

	UFUNCTION(BlueprintCallable)
	void TryToLeave();

	UFUNCTION(BlueprintCallable)
	void AdvanceStage();

	// Se sono ancora al primo Stage e il portinaio non guarda, blocco l'entrata nell'appartamento
	UFUNCTION(BlueprintCallable)
	void EntranceCollision();

	UFUNCTION(BlueprintPure)
	bool IsDoormanWatching() const;

protected:
	virtual void BeginPlay() override;

	void EndLevel();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data|References")
	TObjectPtr<AAshley_Doorman> Doorman;

	// BP_Intercom (resettable)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data|References")
	TObjectPtr<AActor> Intercom;

	// BP_ElectricalPanel (resettable)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data|References")
	TObjectPtr<AActor> ElectricalPanel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data|References")
	TObjectPtr<AAshley_Torch> PhoneTorch;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data|References")
	TObjectPtr<AAshley_Interactable> WindowEntrance;

	// Trigger all'entrata dell'appartamento: al suo OnColliderTrigger chiamo EntranceCollision
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data|References")
	TObjectPtr<AAshley_TriggerCollider> EntranceTrigger;

	// Trigger all'uscita: al suo OnColliderTrigger chiamo TryToLeave
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data|References")
	TObjectPtr<AAshley_TriggerCollider> LeaveTrigger;

	// Item necessari per finire il livello
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data", meta = (GetOptions = "GetItemNames"))
	TArray<FName> KeyItems;
	
	//  **** Level flags ****
	// 
	// After the first dialogue, we unlock the courtyard fence
	UPROPERTY(BlueprintReadWrite)
	bool bFirstDialogueDone = false;

	UPROPERTY(BlueprintReadWrite)
	bool bDoormanTrapped = false;

	// Tutti gli item raccolti, ma lo showcase dell'ultimo e' ancora aperto
	UPROPERTY(BlueprintReadOnly)
	bool bLevelEndPending = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	FGameplayTag EventEndSequence;

	// Alla fine di questa sequenza (parametro di EventEndSequence) resetto il livello
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	FGameplayTag ResetLevelSequenceName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	FGameplayTag EventStartSequence;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	FGameplayTag EventEnterHouse;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	FGameplayTag EventEquipTorch;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	FGameplayTag EventLevelEnd;

	// Livello da caricare all'arrivo di EventLevelEnd
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	TSoftObjectPtr<UWorld> NextLevel;

	// Indice 0: primo reset, poi PlayGenericDialogue per i successivi
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data|Dialogues")
	TObjectPtr<UAshley_GenericDialogue> ResetDialogues;

	// Indice 0: prima del primo reset, 1: dopo, 2: prima volta all'entrata, 3: entrata in casa
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data|Dialogues")
	TObjectPtr<UAshley_GenericDialogue> Dialogues;

	UPROPERTY(BlueprintReadWrite)
	bool bFirstResetDone = false; // TODO: usare solo property per accedere GameManager variabile

	UPROPERTY(BlueprintReadWrite)
	bool bHasPlayedEntranceDialogue = false;

	// Il dialogo di TryToLeave e' in riproduzione: ignoro altri trigger finche' non finisce
	UPROPERTY(BlueprintReadOnly)
	bool bIsLeaveDialoguePlaying = false;

	// Sblocco di sicurezza: se il dialogo viene interrotto da un altro, il suo callback di fine non arriva
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data|Dialogues", meta = (ClampMin = "1.0", Units = "s"))
	float LeaveDialogueMaxLockTime = 10.f;

	UPROPERTY(BlueprintReadWrite)
	int32 CurrentStage = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UAshley_PickupDA> ItemData;

private:

	UFUNCTION()
	TArray<FName> GetItemNames() const;

	void OnLeaveDialogueEnded();

	FTimerHandle TimerHandle_LeaveDialogueLock;
};
