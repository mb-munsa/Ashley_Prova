// Ashley


#include "Interaction/Ashley_Resettable.h"

#include "GameFramework/Actor.h"

void IAshley_Resettable::ResetActor(AActor* Actor)
{
	if (!Actor)
	{
		return;
	}

	if (Actor->Implements<UAshley_Resettable>())
	{
		IAshley_Resettable::Execute_ResetToInitialState(Actor);
		return;
	}

	// TODO: rimuovere quando tutti i BP saranno migrati da BPI_Resettable a Ashley_Resettable
	static const FName LegacyResetName(TEXT("Reset"));
	if (UFunction* LegacyReset = Actor->FindFunction(LegacyResetName))
	{
		if (LegacyReset->NumParms == 0)
		{
			Actor->ProcessEvent(LegacyReset, nullptr);
			return;
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("ResetActor: %s is not resettable"), *Actor->GetName());
}
