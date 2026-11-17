// Ashley


#include "InvestigationBoard/UI/Ashley_InvestigationBoardTabWidget.h"
#include "InvestigationBoard/Ashley_InvestigationBoardDefinition.h"

#include <Components/Button.h>
#include <Components/TextBlock.h>

void UAshley_InvestigationBoardTabWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    if (TabButton)
        TabButton->OnClicked.AddDynamic(this, &ThisClass::HandleClicked);
}

void UAshley_InvestigationBoardTabWidget::Setup(const UInvestigationBoardDefinition* Board)
{
    if (!Board)
        return;

    BoardId = Board->GetBoardId();

    if (TabLabel)
        TabLabel->SetText(Board->Title);
}

void UAshley_InvestigationBoardTabWidget::SetSelected(bool bSelected)
{
    // The active board can't be clicked again
    if (TabButton)
        TabButton->SetIsEnabled(!bSelected);
}

FName UAshley_InvestigationBoardTabWidget::GetBoardId() const
{
    return BoardId;
}

void UAshley_InvestigationBoardTabWidget::HandleClicked()
{
    OnTabClicked.Broadcast(BoardId);
}
