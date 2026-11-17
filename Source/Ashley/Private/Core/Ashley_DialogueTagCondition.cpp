// Ashley


#include "Core/Ashley_DialogueTagCondition.h"

#include "Managers/Ashley_GameStateManager.h"
#include <Kismet/GameplayStatics.h>

bool UAshley_DialogueTagCondition::Evaluate_Implementation(const UObject* WorldContext) const
{
	// richiedo al GameState manager il valore del tag
	auto GameStateManager = UGameplayStatics::GetGameInstance(WorldContext)->GetSubsystem<UAshley_GameStateManager>();

	return GameStateManager->GetTagValue(Tag) == bExpectedValue;
}
