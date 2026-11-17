// Ashley


#include "InvestigationBoard/UI/Ashley_InvestigationDescriptionWidget.h"
#include "InvestigationBoard/Ashley_InvestigationPostIt.h"

#include <Components/TextBlock.h>

void UAshley_InvestigationDescriptionWidget::SetPostIt(const FInvestigationPostIt& PostIt)
{
    PostItId = PostIt.Id;

    if (DescriptionText)
        DescriptionText->SetText(PostIt.Description);

    if (TitleText)
        TitleText->SetText(PostIt.Title);
}

FName UAshley_InvestigationDescriptionWidget::GetPostItId() const
{
    return PostItId;
}

FReply UAshley_InvestigationDescriptionWidget::NativeOnMouseButtonDown(
    const FGeometry& Geometry,
    const FPointerEvent& Event)
{
    if (Event.GetEffectingButton() == EKeys::LeftMouseButton)
        return FReply::Handled();

    return Super::NativeOnMouseButtonDown(Geometry, Event);
}
