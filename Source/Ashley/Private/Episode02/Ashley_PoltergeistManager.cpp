// Ashley


#include "Managers/Ashley_PoltergeistManager.h"

#include "Managers/Ashley_EventManagerSystem.h"
#include "DataAssets/Ashley_MapFearLevels.h"
#include "DataAssets/Ashley_PoltergeistDA.h"
#include <Kismet/GameplayStatics.h>

#include "DebugHelper.h"
#include <Kismet/KismetMathLibrary.h>

void UAshley_PoltergeistManager::InitMap(UAshley_MapFearLevels* Config)
{
	if (Config == nullptr)
	{
		Debug::Print("Settare FearLevels!!");
		return;
	}

	CurrentLevelIndex = 0;
	FearLevels = Config->FearLevels;
	CurrentTimer = 0;
	bIsTimerActive = false;

	// Mi allaccio agli eventi generici
	UGameplayStatics::GetGameInstance(this)->GetSubsystem<UAshley_EventManagerSystem>()->OnEventTriggered.AddDynamic(this, &UAshley_PoltergeistManager::OnPlayerEventTriggered);
}

void UAshley_PoltergeistManager::PlayerTriggerCollider(AAshley_Poltergeist* Poltergeist) const
{
	// Se il livello del poltergeist è più alto del livello corrente, lo ignoro
	if (Poltergeist->GetActivationLevel() > CurrentLevelIndex) return;

	// Se il poltergeist è one_shot, ed è già stato riprodotto, lo ignoro
	if (Poltergeist->IsOneShot() && Poltergeist->HasBeenPlayed()) return;

	// Se il poltergeist è sempre attivo, lo ignoro
	switch (Poltergeist->GetActivationType())
	{
		case EPoltergeistActivationType::ALWAYS_ACTIVE:
				return;
		//case EPoltergeistActivationType::ACTIVATE_BY_PROXIMITY_ALWAYS:
			// Mando l'attivazione
		case EPoltergeistActivationType::ACTIVATE_BY_PROXIMITY_RANDOM:
			auto RandomInt = UKismetMathLibrary::RandomIntegerInRange(0, 100);
			if (RandomInt > Poltergeist->GetActivationProbability())
				return;
	};

	// TODO: valuto se non è passato troppo poco tempo dall'ultimo trigger?

	// Se non è ancora passato il cooldown individuale per poltergeist, lo ignoro
	if (Poltergeist->GetCooldown() + Poltergeist->GetLastActivationTime() > GetWorld()->GetTimeSeconds())
		return;

	// Mando l'attivazione
	Poltergeist->Activate(CurrentLevelIndex);
}

void UAshley_PoltergeistManager::OnPlayerEventTriggered(FGameplayTag EventName, FString EventParameter)
{
	// Se sono già all'ultimo livello, ignoro
	if (CurrentLevelIndex >= FearLevels.Num()) return;

	bool bShouldUpdateLevel = false;
	auto& NextFearLevelConditions = FearLevels[CurrentLevelIndex].UpdateConditions;

	// Controllo se per passare al prossimo livello c'è una condizione di evento
	for (auto& Condition : NextFearLevelConditions)
	{
		if (Condition.UpdateType != ELevelUpdateType::GENERIC_EVENT) continue;

		if (Condition.EventName == EventName && Condition.EventParam == EventParameter)
			bShouldUpdateLevel = true;
	}

	if (bShouldUpdateLevel) UpdateLevel();
}

void UAshley_PoltergeistManager::UpdateLevel()
{
	CurrentLevelIndex++;
	OnLevelIncrement.Broadcast(CurrentLevelIndex);

	Debug::Print(FString::Printf(TEXT("Level INCREmentT! %d"), CurrentLevelIndex));

	// Annullo il timer se sono salito di livello in altro modo
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle_NextLevel);

	// Se sono finiti i livelli non controllo la prossima condizione
	if (CurrentLevelIndex >= FearLevels.Num()) return;

	float Timer = 0;
	auto& NextFearLevelConditions = FearLevels[CurrentLevelIndex].UpdateConditions;

	// Controllo se per passare al prossimo livello c'è una condizione di evento
	for (auto& Condition : NextFearLevelConditions)
	{
		if (Condition.UpdateType == ELevelUpdateType::TIMER) Timer = Condition.Timer;
	}

	// Faccio partire il timer
	GetWorld()->GetTimerManager().SetTimer(
		TimerHandle_NextLevel,
		[this]()
			{ this->UpdateLevel(); },
		Timer,
		false
	);
}

int UAshley_PoltergeistManager::GetCurrentLevel() const
{
	Debug::Print(FString::Printf(TEXT("getting current level %d"), CurrentLevelIndex));

	return CurrentLevelIndex;
}
