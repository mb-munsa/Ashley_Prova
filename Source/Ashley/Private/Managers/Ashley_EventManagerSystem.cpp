// Ashley


#include "Managers/Ashley_EventManagerSystem.h"

void UAshley_EventManagerSystem::BroadcastEventTriggered(FGameplayTag Event, FString EventParam)
{
	OnEventTriggered.Broadcast(Event, EventParam);
}

void UAshley_EventManagerSystem::BroadcastItemSelected(FName ItemName)
{
	OnItemSelected.Broadcast(ItemName);
}
