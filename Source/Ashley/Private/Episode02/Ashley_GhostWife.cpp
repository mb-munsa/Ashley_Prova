// Ashley


#include "Episode02/Ashley_GhostWife.h"
#include "Episode02/Ashley_GhostSpawner.h"
#include <Kismet/GameplayStatics.h>
#include "Managers/Ashley_EventManagerSystem.h"

#include "Tags_Enums_Structs/Ashley_GameplayTags.h"

void AAshley_GhostWife::SetGhostSpawner(TObjectPtr<AAshley_GhostSpawner> GhostController)
{
	GhostSpawner = GhostController;
}

void AAshley_GhostWife::CompleteQuest()
{
	// Mando la cutscene finale
	UGameplayStatics::GetGameInstance(this)->GetSubsystem<UAshley_EventManagerSystem>()
		->BroadcastEventTriggered(FGameplayTag::RequestGameplayTag(FName("Ashley.Events.PlaySequence")), FinalSequenceIndex);

	// Disattivo il ghostSpawner
	GhostSpawner->DisableGhost();

	// Distruggo il fantasma
	this->Destroy();
}
