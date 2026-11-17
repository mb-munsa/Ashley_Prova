// Ashley


#include "Managers/Ashley_QuestManager.h"
#include "Core/Ashley_PlayerController.h"

#include "DebugHelper.h"
#include <Managers/Ashley_WidgetSystem.h>
#include <Kismet/GameplayStatics.h>

// Sets default values
AAshley_QuestManager::AAshley_QuestManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

const TArray<FActiveQuest>& AAshley_QuestManager::GetActiveQuests()
{
	return ActiveQuests;
}

void AAshley_QuestManager::AdvanceQuest(const FName& QuestName, int Step)
{
	//Debug::Print(FString::Printf(TEXT("Advancing quest %s"), *(QuestName.ToString())));

	auto ActiveQuest = ActiveQuests.FindByPredicate([QuestName](auto quest) { return quest.QuestName == QuestName; });

	// Se non la trovo, la aggiungo alle quest attive
	if (ActiveQuest == nullptr)
	{
		FActiveQuest NewQuest;
		NewQuest.QuestName = QuestName;
		int Index = ActiveQuests.Add(NewQuest);
		ActiveQuest = &ActiveQuests[Index];
		if (Step == -1) Step = 0;
	}
	else if (Step == -1)
	{
		Step = ActiveQuest->QuestStep + 1;
	}

	ActiveQuest->QuestStep = Step;
	ActiveQuest->CurrentQuestDesc = QuestConfiguration->GetStepTitleFromQuest(QuestName, Step);
	ActiveQuest->CurrentQuestDialogue = QuestConfiguration->GetDialogueFromQuest(QuestName, Step);

	// Se è settato un dialogo per questo step, lo mostro
	if (!ActiveQuest->CurrentQuestDialogue.IsEmpty())
	{
		auto Player = Cast<AAshley_PlayerController>(GetWorld()->GetFirstPlayerController());
		UGameplayStatics::GetGameInstance(this)->GetSubsystem<UAshley_WidgetSystem>()->SpawnInfoText(ActiveQuest->CurrentQuestDialogue.ToString(), 4);
	}
}

void AAshley_QuestManager::AdvanceQuest(const FName& QuestName, FString StepName)
{
	for (const auto& Quest : QuestConfiguration->Quests)
	{
		if (Quest.QuestID == QuestName)
		{
			int idx = 0;
			for (const auto& QuestStep : Quest.QuestSteps)
			{
				if (QuestStep.StepTitle == StepName)
				{
					AdvanceQuest(QuestName, idx);
					return;
				}
				idx++;
			}
		}
	}
}

void AAshley_QuestManager::CompleteQuest(const FName& QuestName)
{
	auto ActiveQuestIdx = ActiveQuests.IndexOfByPredicate([QuestName](auto quest) { return quest.QuestName == QuestName; });

	if (ActiveQuestIdx != INDEX_NONE) ActiveQuests.RemoveAt(ActiveQuestIdx);
}

// Called when the game starts or when spawned
void AAshley_QuestManager::BeginPlay()
{
	Super::BeginPlay();
	
}


