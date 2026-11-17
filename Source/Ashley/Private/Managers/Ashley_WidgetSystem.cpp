// Ashley


#include "Managers/Ashley_WidgetSystem.h"
#include "Tags_Enums_Structs/Ashley_WidgetData.h"
#include "Characters/Players/Ashley_CharacterPlayer.h"
#include "Core/Ashley_PlayerController.h"
#include "DataAssets/Ashley_WidgetSystemConfigDA.h"
#include "DataAssets/Ashley_DialogueNode.h"

#include "Managers/Ashley_QuestManager.h"

#include "Blueprint/UserWidget.h"
#include "Components/CanvasPanelSlot.h"

#include "Widgets/Ashley_InfoText.h"
#include "Widgets/Ashley_ChoiceMenu.h"
#include "Widgets/Ashley_PauseMenu.h"
#include "Widgets/Ashley_InventoryUI.h"
#include "Widgets/Ashley_SkipCutscene.h"
#include "Widgets/Ashley_BaseHUD.h"
#include "Widgets/Ashley_CollectableNotification.h"

#include <Kismet/GameplayStatics.h>

#include "DebugHelper.h"
#include <Blueprint/WidgetLayoutLibrary.h>

namespace
{
	// Deve superare gli 0.1s che lo showcase aspetta prima di mettere in pausa,
	//  cosi' il timer si congela e la notifica parte solo alla chiusura dello showcase
	constexpr float CollectableNotificationDelay = 0.25f;
	constexpr float CollectableNotificationDuration = 3.0f;

	// Pausa tra una lettera e l'altra nei dialoghi
	constexpr float TypewriterLetterDelay = 0.035f;
}


void UAshley_WidgetSystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	// Non uso PostLoadMap perche' arriva dopo il BeginPlay, quando l'HUD del nuovo livello esiste gia'
	FCoreUObjectDelegates::PreLoadMap.AddUObject(this, &UAshley_WidgetSystem::OnPreLoadMap);

	if (Config.IsValid() || Config.LoadSynchronous())
	{
		//InfoTextBP = Config->InfoTextBP;
		//InteractionPromptBP = Config->InteractionPromptBP;
		//SkipSequenceBP = Config->SkipCutsceneBP;
		PauseMenuBP = Config->PauseMenuBP;
		BaseHUDBP = Config->BaseHUDBP;
		CollectableNotificationBP = Config->CollectableNotificationBP;
		//InventoryBP = Config->InventoryBP;

		//InitializeHUD();
	}
}

void UAshley_WidgetSystem::Deinitialize()
{
	FCoreUObjectDelegates::PreLoadMap.RemoveAll(this);

	Super::Deinitialize();
}

void UAshley_WidgetSystem::OnPreLoadMap(const FString& MapName)
{
	PlayerControllerRef = nullptr;
	CharacterRef = nullptr;

	BaseHUDWidget = nullptr;
	PauseMenuWidget = nullptr;
	InfoTextWidget = nullptr;
	InteractionPromptWidget = nullptr;
	HintWidget = nullptr;
	InventoryWidget = nullptr;
	DiaryWidget = nullptr;
	SkipCutsceneWidget = nullptr;
	CollectableNotificationWidget = nullptr;

	CollectableNotificationQueue.Reset();

	bIsPauseMenuOpen = false;
	bIsInventoryOpen = false;
	bIsDiaryOpen = false;
	bCursorModeOnResume = false;
	bIsShowingDialogue = false;
	bIsShowingInfoText = false;
	CurrentDialogueLine = 0;

	TimerHandle_HideInfoText.Invalidate();
	TimerHandle_HideHint.Invalidate();
	TimerHandle_CollectableNotification.Invalidate();
	TimerHandle_Typewriter.Invalidate();
	TypewriterText.Reset();
	TypewriterVisibleChars = 0;
}

void UAshley_WidgetSystem::InitializeHUD()
{
	UE_LOG(LogTemp, Warning, TEXT("initialize"));

	BaseHUDWidget = CreateWidget<UAshley_BaseHUD>(GetWorld(), BaseHUDBP);
	BaseHUDWidget->AddToViewport(WidgetZOrders[EWidgetType::HUD]);
	BaseHUDWidget->SetVisibility(ESlateVisibility::Visible);
}

void UAshley_WidgetSystem::TogglePauseMenu()
{
	if (PauseMenuWidget == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("CLiccato enter!"));
		PauseMenuWidget = CreateWidget<UAshley_PauseMenu>(GetWorld(), PauseMenuBP);
		PauseMenuWidget->AddToViewport(WidgetZOrders[EWidgetType::PAUSE_MENU]);
	}

	// If pause menu is not already open
	if (!bIsPauseMenuOpen)
	{
		PauseMenuWidget->SetVisibility(ESlateVisibility::Visible);
		bIsPauseMenuOpen = true;

		TogglePause(true);
	}
	// Else, hide menu, cursor and restart game time
	else
	{
		PauseMenuWidget->SetVisibility(ESlateVisibility::Collapsed);
		bIsPauseMenuOpen = false;

		TogglePause(false);
	}
}

void UAshley_WidgetSystem::ToggleInventory()
{
	// Con il menu di pausa aperto inventario e diario non si aprono/chiudono
	if (bIsPauseMenuOpen) return;

	//UE_LOG(LogTemp, Warning, TEXT("Open inventory!"));
	if (InventoryWidget == nullptr)
	{
		InventoryWidget = CreateWidget<UAshley_InventoryUI>(GetWorld(), InventoryBP);
		InventoryWidget->AddToViewport(WidgetZOrders[EWidgetType::INVENTORY]);
	}

	// Se il diario era aperto, lo chiudo
	if (bIsDiaryOpen)
		ToggleDiary();

	bIsInventoryOpen = !bIsInventoryOpen;
	auto NewVisibility = bIsInventoryOpen ? ESlateVisibility::Visible : ESlateVisibility::Collapsed;
	InventoryWidget->SetVisibility(NewVisibility);

	UpdateInventoryUI();

	// Quando apro l'inventario aggiorno anche il log delle quests
	UpdateQuestUI();

	TogglePause(bIsInventoryOpen);

	// evento chiusura inventory
	if (!bIsInventoryOpen)
	{
		OnInventoryClosed.Broadcast();
	}
}

void UAshley_WidgetSystem::ToggleDiary()
{
	if (bIsPauseMenuOpen) return;

	//UE_LOG(LogTemp, Warning, TEXT("Open inventory!"));
	if (DiaryWidget == nullptr)
	{
		DiaryWidget = CreateWidget<UAshley_InventoryUI>(GetWorld(), DiaryBP);
		DiaryWidget->AddToViewport(WidgetZOrders[EWidgetType::INVENTORY]);
	}

	// Se l'inventario era aperto, lo chiudo
	if (bIsInventoryOpen)
		ToggleInventory();

	bIsDiaryOpen = !bIsDiaryOpen;
	auto NewVisibility = bIsDiaryOpen ? ESlateVisibility::Visible : ESlateVisibility::Collapsed;
	DiaryWidget->SetVisibility(NewVisibility);

	UpdateDiaryUI();

	TogglePause(bIsDiaryOpen);

	// evento chiusura inventory
	if (!bIsDiaryOpen)
	{
		OnInventoryClosed.Broadcast();
	}
}

void UAshley_WidgetSystem::UpdateInventoryUI()
{
	if (bIsInventoryOpen) InventoryWidget->UpdateItemsInfo(CharacterRef->GetItemsInInventory());
}

void UAshley_WidgetSystem::UpdateDiaryUI()
{
	if (bIsDiaryOpen) DiaryWidget->UpdateItemsInfo(CharacterRef->GetCollectables());
}

void UAshley_WidgetSystem::UpdateQuestUI()
{
	if (CharacterRef->GetQuestManager() == nullptr) return;
	if (bIsInventoryOpen) InventoryWidget->UpdateQuestsInfo(CharacterRef->GetQuestManager()->GetActiveQuests());
}

void UAshley_WidgetSystem::SpawnInfoText(const FString& TextToDisplay, float Duration, bool bUseTypewriter)
{
	bIsShowingInfoText = true;

	// Un timer rimasto dal testo precedente nasconderebbe questo prima del tempo
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle_HideInfoText);

	StartTypewriter(TextToDisplay);

	// Senza typewriter completo subito la battuta, prima che venga mostrata la prima lettera
	if (!bUseTypewriter) TryCompleteTypewriter();

	if (Duration > -1)
	{
		GetWorld()->GetTimerManager().SetTimer(
			TimerHandle_HideInfoText,
			[this]()
			{
				this->EndDialogue();
			},
			Duration,
			false
		);
	}
}

UAshley_ChoiceMenu* UAshley_WidgetSystem::ShowDialogueChoices(TArray<FDialogueChoice> ChoicesToShow)
{
	bIsShowingInfoText = false;

	GetWorld()->GetTimerManager().ClearTimer(TimerHandle_HideInfoText);
	StopTypewriter();

	// Nascondo eventuale testo
	BaseHUDWidget->DialogueText->SetVisibility(ESlateVisibility::Collapsed);

	// Mostro il widget con le scelte
	BaseHUDWidget->DialogueChoices->ShowChoices(ChoicesToShow);
	BaseHUDWidget->DialogueChoices->SetVisibility(ESlateVisibility::Visible);

	// Attivo il mouse
	PlayerControllerRef->SetShowMouseCursor(true);
	FInputModeGameAndUI InputMode;
	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	PlayerControllerRef->SetInputMode(InputMode);

	return BaseHUDWidget->DialogueChoices;
}

void UAshley_WidgetSystem::HideChoiceMenu()
{
	BaseHUDWidget->DialogueChoices->SetVisibility(ESlateVisibility::Collapsed);
}

void UAshley_WidgetSystem::NextDialogueLine(bool bAutoNext)
{
	if (bIsShowingInfoText)
	{
		Debug::Print("Showing INFO TEXT");

		EndDialogue();
	}

	if (!bIsShowingDialogue)
	{
		Debug::Print("Next line but no dialogue showing!");
		return;
	}

	if (CurrentDialogueLine >= CurrentDialogue.DialogueLines.Num())
	{
		// EndDialogue scarta la callback: lo salvo prima e lo eseguo a dialogo chiuso,
		// cosi' la callback puo' avviare un nuovo dialogo
		FSimpleDelegate OnDialogueEnded = MoveTemp(PendingOnDialogueEnded);
		PendingOnDialogueEnded.Unbind();

		EndDialogue();
		OnDialogueEnded.ExecuteIfBound();
		return;
	}

	auto& DialogueLine = CurrentDialogue.DialogueLines[CurrentDialogueLine].Dialogue;
	auto DialogueDuration = CurrentDialogue.DialogueLines[CurrentDialogueLine].DialogueDuration;

	StartTypewriter(DialogueLine);

	CurrentDialogueLine++;

	// TODO: devo invalidare il timer precedente? se ho fatto skip manuale
	if (bAutoNext)
	{
		GetWorld()->GetTimerManager().SetTimer(
			TimerHandle_HideInfoText,
			[this, bAutoNext]()
			{
				this->NextDialogueLine(bAutoNext);
			},
			DialogueDuration,
			false
		);
	}
}

void UAshley_WidgetSystem::ShowDialogue(const FDialogue& Dialogue)
{
	ShowDialogue(Dialogue, FSimpleDelegate());
}

void UAshley_WidgetSystem::ShowDialogue(const FDialogue& Dialogue, FSimpleDelegate OnDialogueEnded)
{
	// Un eventuale dialogo precedente viene interrotto, il suo callback non viene chiamato
	PendingOnDialogueEnded = MoveTemp(OnDialogueEnded);

	CurrentDialogueLine = 0;
	bIsShowingDialogue = true;
	CurrentDialogue = Dialogue;

	// Se c'era un info text, NextDialogueLine chiamerebbe EndDialogue e il dialogo non partirebbe
	bIsShowingInfoText = false;
	NextDialogueLine(true);
}

void UAshley_WidgetSystem::EndDialogue()
{
	// Dialogo interrotto dall'esterno (sequenze, info text...): il callback non va chiamato
	PendingOnDialogueEnded.Unbind();

	CurrentDialogueLine = 0;
	bIsShowingDialogue = false;
	bIsShowingInfoText = false;

	GetWorld()->GetTimerManager().ClearTimer(TimerHandle_HideInfoText);
	StopTypewriter();
	this->HideWidget(BaseHUDWidget->DialogueText);
	// TODO: check
	this->HideWidget(BaseHUDWidget->DialogueChoices);


	Debug::Print("Dialogue ended!");

	//OnDialogueEnded.Broadcast();

	/// TODO: valutare se restorare qui il tasto interact
	return;
}

void UAshley_WidgetSystem::StartTypewriter(const FString& TextToDisplay)
{
	TypewriterText = TextToDisplay;
	TypewriterVisibleChars = 0;

	// Add to viewport, il testo parte vuoto e si riempie una lettera alla volta
	BaseHUDWidget->DialogueText->SetTextToDisplay(FString());
	BaseHUDWidget->DialogueText->SetVisibility(ESlateVisibility::Visible);

	GetWorld()->GetTimerManager().SetTimer(
		TimerHandle_Typewriter,
		this,
		&UAshley_WidgetSystem::TypeNextLetter,
		TypewriterLetterDelay,
		true,
		0.0f
	);
}

void UAshley_WidgetSystem::TypeNextLetter()
{
	if (BaseHUDWidget == nullptr || TypewriterVisibleChars >= TypewriterText.Len())
	{
		StopTypewriter();
		return;
	}

	// Gli spazi non contano come lettera, altrimenti tra le parole si sente una pausa doppia
	do
	{
		TypewriterVisibleChars++;
	} while (TypewriterVisibleChars < TypewriterText.Len() && FChar::IsWhitespace(TypewriterText[TypewriterVisibleChars - 1]));

	BaseHUDWidget->DialogueText->SetTextToDisplay(TypewriterText.Left(TypewriterVisibleChars));

	if (TypewriterVisibleChars >= TypewriterText.Len())
	{
		StopTypewriter();
	}
}

bool UAshley_WidgetSystem::TryCompleteTypewriter()
{
	if (!GetWorld()->GetTimerManager().IsTimerActive(TimerHandle_Typewriter)) return false;

	StopTypewriter();
	TypewriterVisibleChars = TypewriterText.Len();
	BaseHUDWidget->DialogueText->SetTextToDisplay(TypewriterText);
	return true;
}

void UAshley_WidgetSystem::StopTypewriter()
{
	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().ClearTimer(TimerHandle_Typewriter);
	}
}

void UAshley_WidgetSystem::HideInfoText()
{
	if (BaseHUDWidget->DialogueText != nullptr)
	{
		this->HideWidget(BaseHUDWidget->DialogueText);
	}
}

void UAshley_WidgetSystem::ShowSkipCutscenePrompt(bool bShouldShow)
{
	if (!bShouldShow && BaseHUDWidget->SkipCutscenePrompt != nullptr)
	{
		this->HideWidget(BaseHUDWidget->SkipCutscenePrompt);
	}
	else if (bShouldShow)
	{
		//// Riciclare stesso widget
		//if (BaseHUDWidget->SkipCutscenePrompt == nullptr)
		//{
		//	BaseHUDWidget->SkipCutscenePrompt = CreateWidget<UAshley_SkipCutscene>(GetWorld(), SkipSequenceBP);
		//	BaseHUDWidget->SkipCutscenePrompt->AddToViewport(999);
		//}

		// Add to viewport
		BaseHUDWidget->SkipCutscenePrompt->SetVisibility(ESlateVisibility::Visible);
	}
}

void UAshley_WidgetSystem::UpdateSkipCutscenePrompt(float NewPercentage)
{
	BaseHUDWidget->SkipCutscenePrompt->UpdatePercentage(NewPercentage);
}

void UAshley_WidgetSystem::ShowInteractionPrompt(const FString& TextToDisplay, FVector InteractionPosition)
{
	//TODO: remove
	return;
}

void UAshley_WidgetSystem::HideInteractionPrompt()
{
	if (BaseHUDWidget->InteractionPrompt)
		HideWidget(BaseHUDWidget->InteractionPrompt);
}

void UAshley_WidgetSystem::ShowHint(const FString& TextToDisplay, float Duration)
{
	//if (BaseHUDWidget->HintPrompt == nullptr)
	//{
	//	BaseHUDWidget->HintPrompt = CreateWidget<UAshley_InfoText>(GetWorld(), InteractionPromptBP);
	//	BaseHUDWidget->HintPrompt->AddToViewport(WidgetZOrders[EWidgetType::INTERACTION_PROMPT]);
	//}

	// Add to viewport
	BaseHUDWidget->HintPrompt->SetTextToDisplay(TextToDisplay);
	BaseHUDWidget->HintPrompt->SetVisibility(ESlateVisibility::Visible);

	if (Duration > 0)
	{
		GetWorld()->GetTimerManager().SetTimer(
			TimerHandle_HideHint,
			[this]()
			{
				this->HideHint();
			},
			Duration,
			false
		);
	}
}

void UAshley_WidgetSystem::HideHint()
{
	if (BaseHUDWidget->HintPrompt)
	{
		HideWidget(BaseHUDWidget->HintPrompt);
		TimerHandle_HideHint.Invalidate();
	}
}

void UAshley_WidgetSystem::ShowCollectableNotification(FName ItemID)
{
	CollectableNotificationQueue.Add(ItemID);

	// Se il timer e' attivo c'e' gia' una notifica in corso, che alla fine passera' alla prossima in coda
	if (GetWorld()->GetTimerManager().IsTimerActive(TimerHandle_CollectableNotification)) return;

	GetWorld()->GetTimerManager().SetTimer(
		TimerHandle_CollectableNotification,
		[this]()
		{
			this->ShowNextCollectableNotification();
		},
		CollectableNotificationDelay,
		false
	);
}

void UAshley_WidgetSystem::ShowNextCollectableNotification()
{
	if (!CollectableNotificationBP)
	{
		UE_LOG(LogTemp, Error, TEXT("CollectableNotificationBP non assegnata nel config, nessuna notifica"));
		CollectableNotificationQueue.Reset();
		return;
	}

	if (CollectableNotificationWidget == nullptr)
	{
		CollectableNotificationWidget = CreateWidget<UAshley_CollectableNotification>(GetWorld(), CollectableNotificationBP);
		CollectableNotificationWidget->AddToViewport(WidgetZOrders[EWidgetType::NOTIFICATION]);
	}

	// Salto gli item senza icona
	while (!CollectableNotificationQueue.IsEmpty())
	{
		const FName ItemID = CollectableNotificationQueue[0];
		CollectableNotificationQueue.RemoveAt(0);

		if (!CollectableNotificationWidget->SetItem(ItemID)) continue;

		// Non deve bloccare i click, nel minivan il gioco e' in Game+UI
		CollectableNotificationWidget->SetVisibility(ESlateVisibility::HitTestInvisible);
		CollectableNotificationWidget->PlayShowAnimation();

		GetWorld()->GetTimerManager().SetTimer(
			TimerHandle_CollectableNotification,
			[this]()
			{
				this->HideCollectableNotification();
			},
			CollectableNotificationDuration,
			false
		);
		return;
	}

	HideWidget(CollectableNotificationWidget);
}

void UAshley_WidgetSystem::HideCollectableNotification()
{
	const float FadeDuration = CollectableNotificationWidget->PlayHideAnimation();

	// SetTimer con durata 0 non scatterebbe mai
	GetWorld()->GetTimerManager().SetTimer(
		TimerHandle_CollectableNotification,
		[this]()
		{
			this->ShowNextCollectableNotification();
		},
		FMath::Max(FadeDuration, 0.01f),
		false
	);
}

// TODO: FIX
void UAshley_WidgetSystem::SetPlayerRef(AAshley_PlayerController* PlayerRef)
{
	PlayerControllerRef = PlayerRef;
	CharacterRef = Cast<AAshley_CharacterPlayer>(PlayerRef->GetCharacter());
}

// Setto l'inventario a seconda del personaggio
void UAshley_WidgetSystem::SetInventoryUI(TSubclassOf<UAshley_InventoryUI> InventoryPrefab)
{
	InventoryBP = InventoryPrefab;
}

void UAshley_WidgetSystem::SetDiaryUI(TSubclassOf<UAshley_InventoryUI> DiaryPrefab)
{
	DiaryBP = DiaryPrefab;
}

void UAshley_WidgetSystem::HideWidget(UUserWidget* Widget)
{
	if (Widget) Widget->SetVisibility(ESlateVisibility::Collapsed);
}

void UAshley_WidgetSystem::TogglePause(bool bShouldPause)
{
	// Chiudendo un menu sopra un altro (es. pausa sopra l'inventario) resto in pausa col cursore
	if (!bShouldPause && IsAnyMenuOpen())
		bShouldPause = true;

	if (bShouldPause)
	{
		PlayerControllerRef->SetShowMouseCursor(true);
		FInputModeGameAndUI InputMode;
		InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
		PlayerControllerRef->SetInputMode(InputMode);

		PlayerControllerRef->SetPause(true);
	}
	else if (bCursorModeOnResume)
	{
		PlayerControllerRef->SetShowMouseCursor(true);
		FInputModeGameAndUI InputMode;
		InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
		InputMode.SetHideCursorDuringCapture(false);
		PlayerControllerRef->SetInputMode(InputMode);

		PlayerControllerRef->SetPause(false);
	}
	else
	{
		PlayerControllerRef->SetShowMouseCursor(false);
		FInputModeGameOnly inputModeGame;
		PlayerControllerRef->SetInputMode(inputModeGame);

		PlayerControllerRef->SetPause(false);
	}
}

