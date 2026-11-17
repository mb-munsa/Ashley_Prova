// Ashley


#include "Managers/Ashley_GameStateManager.h"

void UAshley_GameStateManager::SetTag(FGameplayTag Tag, bool bEnabled)
{
	if (GameStateTags.Contains(Tag))
		GameStateTags[Tag] = bEnabled;
	else
		GameStateTags.Add(Tag, bEnabled);
}

bool UAshley_GameStateManager::GetTagValue(FGameplayTag Tag)
{
	if (GameStateTags.Contains(Tag))
		return GameStateTags[Tag];
	return false;
}
