// Ashley


#include "Episode01/Ashley_ApartmentLevelManager.h"

#include "Components/BoxComponent.h"
#include <Kismet/GameplayStatics.h>

#include "Episode01/Ashley_Doorman.h"
#include "Core/Ashley_PlayerController.h"
#include "Interaction/Ashley_Resettable.h"
#include "Managers/Ashley_GameStateManager.h"
#include "Characters/Players/Ashley_CharacterPlayer.h"
#include "DataAssets/Ashley_GenericDialogue.h"
#include "Interaction/Items/Ashley_Torch.h"
#include "DataAssets/Ashley_PickupDA.h"
#include "Interaction/Items/Ashley_Pickup.h"
#include "Interaction/Ashley_Interactable.h"
#include "Core/Ashley_TriggerCollider.h"
#include <Managers/Ashley_EventManagerSystem.h>
#include "Managers/Ashley_LevelTransitionSubsystem.h"
#include "Tags_Enums_Structs/Ashley_GameplayTags.h"
#include "MyBlueprintFunctionLibrary.h"

#include "DebugHelper.h"

AAshley_ApartmentLevelManager::AAshley_ApartmentLevelManager()
{
	PrimaryActorTick.bCanEverTick = false;

	EventLevelEnd = AshleyGameplayTags::Ashley_Events_LevelEnd;
	EventEndSequence = AshleyGameplayTags::Ashley_Events_EndSequence;
}

void AAshley_ApartmentLevelManager::BeginPlay()
{
	Super::BeginPlay();

	auto EventManager = UGameplayStatics::GetGameInstance(this)->GetSubsystem<UAshley_EventManagerSystem>();
	EventManager->OnEventTriggered.AddDynamic(this, &AAshley_ApartmentLevelManager::OnPlayerEventTriggered);

	AAshley_Pickup::OnItemPickup.AddUObject(this, &AAshley_ApartmentLevelManager::OnPlayerItemPickup);

	if (EntranceTrigger)
	{
		EntranceTrigger->OnColliderTrigger.AddDynamic(this, &AAshley_ApartmentLevelManager::EntranceCollision);
	}

	if (LeaveTrigger)
	{
		LeaveTrigger->OnColliderTrigger.AddDynamic(this, &AAshley_ApartmentLevelManager::TryToLeave);
	}
}

void AAshley_ApartmentLevelManager::EndLevel()
{
	Debug::Print("All items got, ending level!");

	// TODO: mandare prima la cutscene di fine livello, e lanciare l'evento alla sua fine
	UGameplayStatics::GetGameInstance(this)->GetSubsystem<UAshley_EventManagerSystem>()->BroadcastEventTriggered(EventLevelEnd, FString());
}

void AAshley_ApartmentLevelManager::OnPlayerEventTriggered(FGameplayTag EventName, FString EventParam)
{
	// All'entrata in casa
	if (EventName == EventEnterHouse)
	{
		Dialogues->PlayDialogue(GetGameInstance(), 3);

		// Equipaggio e accendo la torcia
		PhoneTorch->Interact_Implementation();
		PhoneTorch->TurnOn();

		// Disattivo l'interazione con la finestra, a prescindere di come sono entrato
		WindowEntrance->ToggleInteraction(false);
	}
	else if (bLevelEndPending && EventName == AshleyGameplayTags::Ashley_Events_ShowcaseClosed)
	{
		bLevelEndPending = false;
		EndLevel();
	}
	else if (EventName == EventLevelEnd)
	{
		GetGameInstance()->GetSubsystem<UAshley_LevelTransitionSubsystem>()->TravelToLevel(NextLevel);
	}
	else if (EventName == EventEndSequence && ResetLevelSequenceName.IsValid())
	{
		// Il parametro dell'evento e' il nome (tag) della sequenza appena terminata
		FGameplayTag SequenceTag;
		if (UMyBlueprintFunctionLibrary::TryRequestGameplayTag(FName(*EventParam), SequenceTag) && SequenceTag == ResetLevelSequenceName)
		{
			ResetLevel();
		}
	}
}

// Ascolto gli eventi di pickup item, per decidere quando mandare la cutscene di fine livello
void AAshley_ApartmentLevelManager::OnPlayerItemPickup(FName ItemID)
{
	if (KeyItems.Contains(ItemID))
	{
		KeyItems.Remove(ItemID);
		// Quando ho raccolto tutti gli item necessari, mando la fine del livello
		if (KeyItems.Num() <= 0)
		{
			// Lo showcase mette in pausa il gioco: aspetto che il player lo chiuda
			auto Character = Cast<AAshley_CharacterPlayer>(UGameplayStatics::GetPlayerCharacter(this, 0));
			if (Character && Character->IsShowcaseShowing())
				bLevelEndPending = true;
			else
				EndLevel();
		}
	}
}

void AAshley_ApartmentLevelManager::SetFirstDialogueDone()
{
	if (bFirstDialogueDone) return;

	bFirstDialogueDone = true;

	ActivateCourtyard();
}

void AAshley_ApartmentLevelManager::ActivateElectricalPanel()
{
	// TODO: Attivo doorman, valutare miglioramento
	Doorman->GoToCheckElectricalPanel();
}

void AAshley_ApartmentLevelManager::ResetLevel()
{
	// TODO: migliorare
	WindowEntrance->ToggleInteraction(true);

	IAshley_Resettable::ResetActor(Doorman);

	if (AAshley_PlayerController* PlayerController = Cast<AAshley_PlayerController>(UGameplayStatics::GetPlayerController(this, 0)))
	{
		PlayerController->TriggerPlayerReset();
	}

	IAshley_Resettable::ResetActor(Intercom);
	IAshley_Resettable::ResetActor(ElectricalPanel);

	if (!ResetDialogues)
	{
		return;
	}

	if (bFirstResetDone)
	{
		ResetDialogues->PlayGenericDialogue(GetGameInstance(), 0);
	}
	else
	{
		ResetDialogues->PlayDialogue(GetGameInstance(), 0);
		GetGameInstance()->GetSubsystem<UAshley_GameStateManager>()->SetTag(AshleyGameplayTags::Ashley_GameState_FirstResetDone, true);
		bFirstResetDone = true;
	}
}

void AAshley_ApartmentLevelManager::ActivateIntercom()
{
	if (Doorman)
	{
		WindowEntrance->ToggleInteraction(false);
		Doorman->GoCheckIntercom();
	}
}

void AAshley_ApartmentLevelManager::TryToLeave()
{
	// Evito lo spam del dialogo mentre il player continua a colpire il collider
	if (!Dialogues || bIsLeaveDialoguePlaying)
		return;

	bIsLeaveDialoguePlaying = true;

	Dialogues->PlayDialogue(GetGameInstance(), bFirstResetDone ? 1 : 0,
		FSimpleDelegate::CreateUObject(this, &AAshley_ApartmentLevelManager::OnLeaveDialogueEnded));

	// Se il dialogo viene interrotto (sequenza, altro dialogo...) il callback non viene chiamato: sblocco comunque
	GetWorldTimerManager().SetTimer(TimerHandle_LeaveDialogueLock, this,
		&AAshley_ApartmentLevelManager::OnLeaveDialogueEnded, LeaveDialogueMaxLockTime, false);
}

void AAshley_ApartmentLevelManager::OnLeaveDialogueEnded()
{
	bIsLeaveDialoguePlaying = false;
	GetWorldTimerManager().ClearTimer(TimerHandle_LeaveDialogueLock);
}

void AAshley_ApartmentLevelManager::AdvanceStage()
{
	++CurrentStage;

	if (CurrentStage >= 0 && CurrentStage <= 2)
	{
		IAshley_Resettable::ResetActor(Doorman);
	}
}

void AAshley_ApartmentLevelManager::EntranceCollision()
{
	if (!Dialogues)
		return;

	if (!bHasPlayedEntranceDialogue)
	{
		Dialogues->PlayDialogue(GetGameInstance(), 2);
		bHasPlayedEntranceDialogue = true;
		return;
	}

	if (CurrentStage == 0 && !IsDoormanWatching())
	{
		Dialogues->PlayDialogue(GetGameInstance(), 1);
		ResetLevel();
	}
}

bool AAshley_ApartmentLevelManager::IsDoormanWatching() const
{
	return Doorman && Doorman->IsDoormanWatching();
}

TArray<FName> AAshley_ApartmentLevelManager::GetItemNames() const
{
	TArray<FName> ItemsNames;

	if (ItemData)
	{
		for (const auto& PickUp : ItemData->PickupItemsData)
		{
			ItemsNames.Add(PickUp.ItemID);
		}
	}

	return ItemsNames;
}
