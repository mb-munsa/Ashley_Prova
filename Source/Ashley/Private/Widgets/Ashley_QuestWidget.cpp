// Ashley


#include "Widgets/Ashley_QuestWidget.h"
#include "Components/TextBlock.h"

void UAshley_QuestWidget::SetQuestName(const FString& TextToDisplay)
{
	QuestName->SetText(FText::FromString(TextToDisplay));
}

void UAshley_QuestWidget::SetQuestText(const FString& TextToDisplay)
{
	QuestText->SetText(FText::FromString(TextToDisplay));
}
