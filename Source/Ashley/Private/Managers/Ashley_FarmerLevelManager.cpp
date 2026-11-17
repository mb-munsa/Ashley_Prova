// Ashley

#include "Episode02/Ashley_FarmerLevelManager.h"
#include "Episode02/Ashley_GhostSpawner.h"
#include "Episode02/Ashley_GhostWife.h"
#include "Core/Ashley_TriggerCollider.h"
#include "Core/Ashley_PlayerController.h"
#include "Interaction/Items/Ashley_Pickup.h"
#include "Interaction/Ashley_Door.h"
#include "Characters/Players/Ashley_FarmerPlayer.h"
#include "Components/Ashley_InventoryComponent.h"

#include "DataAssets/Ashley_GenericDialogue.h"
#include "DataAssets/Ashley_PickupDA.h"
#include "Managers/Ashley_EventManagerSystem.h"
#include "Managers/Ashley_WidgetSystem.h"
#include "Managers/Ashley_PoltergeistManager.h"
#include "DataAssets/Ashley_MapFearLevels.h"
#include <Kismet/GameplayStatics.h>

#include "DebugHelper.h"

AAshley_FarmerLevelManager::AAshley_FarmerLevelManager()
{
}

void AAshley_FarmerLevelManager::OnItemPickupHandler(FName ItemID)
{
	// Quando raccolgo la tanica, attivo il trigger per la cutscene del fienile
	if (ItemID == PetrolTank)
	{
		SecondSequenceTrigger->SetActive(true);
	}
}

void AAshley_FarmerLevelManager::OnPlayerEventTriggered(FGameplayTag EventName, FString EventParam)
{
	// All'entrata in casa, se il livello di paura è abbastanza alto, spawno il fantasma moglie
	if (EventName == EventEnterHouse)
	{
		if (EventParam == "Entered")
		{
			if (!bHasSpawnedWife && PoltergeistManager->GetCurrentLevel() >= MinFearLevel)
			{
				bHasSpawnedWife = true;
				// Mando cutscene fantasma moglie apparsa
				UGameplayStatics::GetGameInstance(this)->
					GetSubsystem<UAshley_EventManagerSystem>()->BroadcastEventTriggered(EventStartSequence, WifeAppearSequenceIndex.ToString());
			}
		}
		// La prima volta che esco di casa, mostro prompt per shift = correre
		else
		{
			if (!bHasShowedRunPrompt)
			{
				UGameplayStatics::GetGameInstance(this)->
					GetSubsystem<UAshley_WidgetSystem>()->ShowHint("Press SHIFT to Run");
				bHasShowedRunPrompt = true;

				// Abilito lo sprint
				auto PlayerControllerRef = Cast<AAshley_PlayerController>(GetWorld()->GetFirstPlayerController());
				PlayerControllerRef->ToggleSprintBlock(false);
			}
		}
	}

	// End Cutscenes
	else if (EventName == EventEndSequence)
	{
		FGameplayTag SequenceName = FGameplayTag::RequestGameplayTag(FName(*EventParam));

		// Alla fine della cutscene iniziale, faccio partire il primo dialogo
		if (SequenceName == TelephoneSequenceIndex)
		{
			TelephoneDialogue->PlayDialogue(GetGameInstance());
		}
		// Alla fine della cutscene del buco nero faccio partire il dialogo
		else if (SequenceName == BlackHoleSequenceIndex)
		{
			BlackHoleDialogue->PlayDialogue(GetGameInstance());
			UGameplayStatics::GetGameInstance(this)->
				GetSubsystem<UAshley_EventManagerSystem>()->BroadcastEventTriggered(EventEquipTorch, "");
		}
		// Alla fine della cutscene del fienile, spawno il fantasma
		else if (SequenceName == BarnSequenceIndex)
		{
			GhostSpawner->SpawnGhostAtLocation(GhostSpawnPointBarn, GhostDuration, true);
		}
		// TODO: fare anche per cutscene garage
		else if (SequenceName == GarageSequenceIndex)
		{
			GhostSpawner->SpawnGhostAtLocation(GhostSpawnPointGarage, GhostDuration, false);
		}
		// Alla fine della cutscene di comparsa moglie, spawno il fantasma NPC
		else if (SequenceName == WifeAppearSequenceIndex)
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
			auto GhostWife = GetWorld()->SpawnActor<AAshley_GhostWife>(GhostWifePrefab, GhostWifeSpawn->GetActorLocation(), GhostWifeSpawn->GetActorRotation(), SpawnParams);
			GhostWife->SetGhostSpawner(GhostSpawner);
		}

	}

	// Start Cutscenes
	else if (EventName == EventStartSequence)
	{
		FGameplayTag SequenceName = FGameplayTag::RequestGameplayTag(FName(*EventParam));

		// Mi assicuro che la porta sia chiusa, dopo la cutscene
		if ((SequenceName == BlackHoleSequenceIndex) && !bHasSequenceDoorPlayed)
		{
			HouseDoor->SwitchToClosed();
			bHasSequenceDoorPlayed = true;
		}
	}
}

void AAshley_FarmerLevelManager::BeginPlay()
{
	auto EventManager = UGameplayStatics::GetGameInstance(this)->GetSubsystem<UAshley_EventManagerSystem>();
	EventManager->OnEventTriggered.AddDynamic(this, &AAshley_FarmerLevelManager::OnPlayerEventTriggered);
	AAshley_Pickup::OnItemPickup.AddUObject(this, &AAshley_FarmerLevelManager::OnItemPickupHandler);

	// Setto i valori del poltergeist manager
	PoltergeistManager = UGameplayStatics::GetGameInstance(this)->GetSubsystem<UAshley_PoltergeistManager>();
	PoltergeistManager->InitMap(FearLevels);
}

TArray<FName> AAshley_FarmerLevelManager::GetValidIDs() const
{
	UAshley_PickupDA* LoadedAsset;
	if (PickupDataAsset.IsValid())
	{
		LoadedAsset = PickupDataAsset.Get();
	}
	else
	{
		LoadedAsset = PickupDataAsset.LoadSynchronous();
	}
	TArray<FName> IDs;
	if (LoadedAsset)
	{
		IDs = LoadedAsset->GetIDsValids();
	}
	return IDs;
}
