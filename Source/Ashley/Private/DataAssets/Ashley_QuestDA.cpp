// Ashley


#include "DataAssets/Ashley_QuestDA.h"

const FString& UAshley_QuestDA::GetStepTitleFromQuest(FName QuestName, int QuestStep)
{
	for (const auto& Quest : Quests)
	{
		if (Quest.QuestID == QuestName)
		{
			return Quest.QuestSteps[QuestStep].StepTitle;
		}
	}
	static const FString EmptyString = TEXT("");
	return EmptyString;
}

const FText& UAshley_QuestDA::GetDialogueFromQuest(FName QuestName, int QuestStep)
{
	for (const auto& Quest : Quests)
	{
		if (Quest.QuestID == QuestName)
		{
			return Quest.QuestSteps[QuestStep].StepDialogue;
		}
	}
	return FText::GetEmpty();
}
