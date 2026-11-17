// Ashley


#include "InvestigationBoard/UI/Ashley_InvestigationPostItWidget.h"
#include "InvestigationBoard/UI/Ashley_InvestigationBoardWidget.h"

#include <Components/VerticalBox.h>
#include <Components/Image.h>
#include <Components/Button.h>
#include <Components/TextBlock.h>

#include "DebugHelper.h"

void UAshley_InvestigationPostItWidget::NativeConstruct()
{
    Super::NativeConstruct();

    //PostItButton->OnClicked.AddDynamic(
    //    this, &UAshley_InvestigationPostItWidget::OnPostItClicked);

    //PinButton->OnPressed.AddDynamic(
    //    this, &UAshley_InvestigationPostItWidget::OnPinPressed);

    //PinButton->OnReleased.AddDynamic(
    //    this, &UAshley_InvestigationPostItWidget::OnPinReleased);
}

void UAshley_InvestigationPostItWidget::OnPostItClicked()
{
    // The description lives on the board, above post-its and threads
    if (BoardWidget)
        BoardWidget->TogglePostItDescription(this);
}

//void UAshley_InvestigationPostItWidget::OnPinPressed()
//{
//    Debug::Print("Pressed pin");
//
//    bDraggingPin = true;
//    // Begin connection attempt.
//    if (BoardWidget)
//    {
//        BoardWidget->BeginThreadDrag(this);
//    }
//}
//
//void UAshley_InvestigationPostItWidget::OnPinReleased()
//{
//    if (!bDraggingPin)
//        return;
//
//    bDraggingPin = false;
//    // End connection attempt; target-pin detection goes here.
//}

FReply UAshley_InvestigationPostItWidget::NativeOnMouseButtonDown(
    const FGeometry& Geometry,
    const FPointerEvent& Event)
{
    Debug::Print("Pressed postIt");

    // Right click is left to the board (panning)
    if (Event.GetEffectingButton() != EKeys::LeftMouseButton)
        return Super::NativeOnMouseButtonDown(Geometry, Event);

    if (PinImage->GetCachedGeometry().IsUnderLocation(
            Event.GetScreenSpacePosition()))
    {
        Debug::Print("Pressed pin");


        if (BoardWidget)
        {
            BoardWidget->BeginThreadDrag(this);

            return FReply::Handled().CaptureMouse(
                BoardWidget->TakeWidget());
        }
    }
    else
    {
        OnPostItClicked();
    }

    // Handled, otherwise the click reaches the board and closes the description
    return FReply::Handled();
}

void UAshley_InvestigationPostItWidget::SetPostIt(
    const FInvestigationPostIt& PostIdDefinition, 
    UAshley_InvestigationBoardWidget* BoardWidgetRef)
{
    BoardWidget = BoardWidgetRef;
    PostItData = PostIdDefinition;

    PostItTitle->SetText(PostIdDefinition.Title);

    if (PostIdDefinition.Image)
    {
        PostItImage->SetBrushFromTexture(PostIdDefinition.Image);
        PostItImage->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
    }
    else
    {
        PostItImage->SetVisibility(ESlateVisibility::Collapsed);
    }
}

FGeometry UAshley_InvestigationPostItWidget::GetPinGeometry() const
{
    return PinImage->GetCachedGeometry();
}

FName UAshley_InvestigationPostItWidget::GetPostItId() const
{
    return PostItData.Id;
}
