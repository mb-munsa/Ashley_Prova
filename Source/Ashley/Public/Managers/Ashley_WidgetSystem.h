// Ashley

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include <DataAssets/Ashley_WidgetSystemConfigDA.h>
#include <DataAssets/Ashley_CutsceneDialogueDA.h>
#include <DataAssets/Ashley_GenericDialogue.h>

#include "Ashley_WidgetSystem.generated.h"

class AAshley_CharacterPlayer;
class UAshley_ChoiceMenu;
struct FDialogueChoice;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInventoryClosed);
//DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDialogueEnded);

/**
 * 
 */
UCLASS(Config = Game)
class ASHLEY_API UAshley_WidgetSystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	UPROPERTY(BlueprintAssignable)
	FOnInventoryClosed OnInventoryClosed;

	//UPROPERTY(BlueprintAssignable)
	//FOnDialogueEnded OnDialogueEnded;

	UPROPERTY(Config)
	TSoftObjectPtr <UAshley_WidgetSystemConfigDA> Config;

	UFUNCTION()
	void InitializeHUD();

	UFUNCTION(BlueprintCallable)
	void TogglePauseMenu();

	UFUNCTION(BlueprintCallable)
	void ToggleInventory();

	bool IsInventoryOpen() const { return bIsInventoryOpen; }

	// Pausa, inventario o diario aperti: i click non devono arrivare agli attori nel mondo
	bool IsAnyMenuOpen() const { return bIsPauseMenuOpen || bIsInventoryOpen || bIsDiaryOpen; }

	// Livelli giocati col mouse (es. minivan): alla chiusura di pausa/inventario/diario
	// rimette Game+UI con cursore visibile invece di GameOnly. Si resetta al cambio livello
	UFUNCTION(BlueprintCallable)
	void SetCursorModeOnResume(bool bKeepCursor) { bCursorModeOnResume = bKeepCursor; }

	UFUNCTION(BlueprintCallable)
	void ToggleDiary();

	UFUNCTION(BlueprintCallable)
	void UpdateInventoryUI();

	UFUNCTION(BlueprintCallable)
	void UpdateDiaryUI();

	UFUNCTION()
	void UpdateQuestUI();

	UFUNCTION(BlueprintCallable)
	void SpawnInfoText(const FString& TextToDisplay, float Duration = 8.0f, bool bUseTypewriter = true);

	UFUNCTION(BlueprintCallable)
	UAshley_ChoiceMenu* ShowDialogueChoices(TArray<FDialogueChoice>  ChoicesToShow);

	UFUNCTION(BlueprintCallable)
	void HideChoiceMenu();

	UFUNCTION(BlueprintCallable)
	void ShowDialogue(const FDialogue& Dialogue);

	// OnDialogueEnded viene chiamato quando l'ultima battuta viene nascosta (timer o skip del giocatore),
	// non se il dialogo viene interrotto da EndDialogue o da un altro ShowDialogue
	void ShowDialogue(const FDialogue& Dialogue, FSimpleDelegate OnDialogueEnded);

	UFUNCTION()
	void NextDialogueLine(bool bAutoNext = false);

	UFUNCTION()
	void HideInfoText();

	// Se la battuta si sta ancora scrivendo la mostra tutta e ritorna true
	UFUNCTION()
	bool TryCompleteTypewriter();

	UFUNCTION(BlueprintPure)
	bool IsDialogueActive() const { return bIsShowingDialogue || bIsShowingInfoText; }

	UFUNCTION()
	void EndDialogue();

	UFUNCTION()
	void ShowSkipCutscenePrompt(bool bShouldShow);

	UFUNCTION()
	void UpdateSkipCutscenePrompt(float Percentage);

	UFUNCTION()
	void ShowInteractionPrompt(const FString& TextToDisplay, FVector InteractionPosition);

	UFUNCTION()
	void HideInteractionPrompt();

	UFUNCTION()
	void ShowHint(const FString& TextToDisplay, float Duration = 4.0f);

	UFUNCTION()
	void HideHint();

	UFUNCTION()
	void ShowCollectableNotification(FName ItemID);

	UFUNCTION()
	void SetPlayerRef(AAshley_PlayerController* PlayerRef);

	UFUNCTION()
	void SetInventoryUI(TSubclassOf<UAshley_InventoryUI> InventoryPrefab);

	UFUNCTION()
	void SetDiaryUI(TSubclassOf<UAshley_InventoryUI> DiaryPrefab);


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets Prefabs")
	TSubclassOf<UAshley_PauseMenu> PauseMenuBP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets Prefabs")
	TSubclassOf<UAshley_InventoryUI> InventoryBP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets Prefabs")
	TSubclassOf<UAshley_InventoryUI> DiaryBP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets Prefabs")
	TSubclassOf<UAshley_BaseHUD> BaseHUDBP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets Prefabs")
	TSubclassOf<UAshley_CollectableNotification> CollectableNotificationBP;

	UPROPERTY(BlueprintReadOnly)
	UAshley_BaseHUD* BaseHUDWidget;

protected:

	// I widget e i riferimenti al player non sopravvivono al cambio livello
	void OnPreLoadMap(const FString& MapName);

	UFUNCTION()
	void HideWidget(UUserWidget* Widget);
	UFUNCTION()
	void TogglePause(bool bShouldPause);
	UFUNCTION()
	void ShowNextCollectableNotification();
	UFUNCTION()
	void HideCollectableNotification();
	UFUNCTION()
	void StartTypewriter(const FString& TextToDisplay);
	UFUNCTION()
	void TypeNextLetter();
	UFUNCTION()
	void StopTypewriter();

	UPROPERTY()
	AAshley_PlayerController* PlayerControllerRef;
	UPROPERTY()
	AAshley_CharacterPlayer* CharacterRef;
	UPROPERTY()
	bool bIsPauseMenuOpen;
	UPROPERTY()
	bool bIsInventoryOpen;
	UPROPERTY()
	bool bIsDiaryOpen;
	UPROPERTY()
	bool bCursorModeOnResume = false;
	UPROPERTY()
	int CurrentDialogueLine = 0;
	FDialogue CurrentDialogue;
	bool bIsShowingDialogue;
	FSimpleDelegate PendingOnDialogueEnded;

	// TODO: temp, convertire tutti in FDIALOGUE
	bool bIsShowingInfoText;

	UPROPERTY()
	FTimerHandle TimerHandle_HideInfoText;
	UPROPERTY()
	FTimerHandle TimerHandle_HideHint;
	UPROPERTY()
	FTimerHandle TimerHandle_CollectableNotification;
	UPROPERTY()
	FTimerHandle TimerHandle_Typewriter;

	// Testo completo della battuta e quante lettere ne sono gia' visibili
	FString TypewriterText;
	int32 TypewriterVisibleChars = 0;

	UPROPERTY()
	TArray<FName> CollectableNotificationQueue;

	// Instanced widgets

	UPROPERTY()
	UAshley_PauseMenu* PauseMenuWidget;
	UPROPERTY()
	UAshley_InfoText* InfoTextWidget;
	UPROPERTY()
	UAshley_InfoText* InteractionPromptWidget;
	UPROPERTY()
	UAshley_InfoText* HintWidget;
	UPROPERTY()
	UAshley_InventoryUI* InventoryWidget;
	UPROPERTY()
	UAshley_InventoryUI* DiaryWidget;
	UPROPERTY()
	UAshley_SkipCutscene* SkipCutsceneWidget;
	UPROPERTY()
	UAshley_CollectableNotification* CollectableNotificationWidget;
};
