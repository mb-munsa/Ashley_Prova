// Ashley


#include "Characters/Players/Ashley_JournalistPlayer.h"
#include <Managers/Ashley_WidgetSystem.h>

#include <Kismet/GameplayStatics.h>


void AAshley_JournalistPlayer::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	UGameplayStatics::GetGameInstance(this)->GetSubsystem<UAshley_WidgetSystem>()->SetDiaryUI(DiaryBP);
}
