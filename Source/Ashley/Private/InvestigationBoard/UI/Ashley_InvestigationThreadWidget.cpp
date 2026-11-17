// Ashley


#include "InvestigationBoard/UI/Ashley_InvestigationThreadWidget.h"
#include "Rendering/DrawElements.h"

#include "Components/CanvasPanelSlot.h"
#include "Components/Image.h"

void UAshley_InvestigationThreadWidget::SetEndpoints(
    FVector2D Start, FVector2D End)
{
    StartPoint = Start;
    EndPoint = End;
    Update(StartPoint, EndPoint);
}

void UAshley_InvestigationThreadWidget::SetTint(const FLinearColor& Tint)
{
    if (ThreadImage)
        ThreadImage->SetColorAndOpacity(Tint);
}

void UAshley_InvestigationThreadWidget::Update(
    FVector2D Start, FVector2D End)
{
    const FVector2D Delta = End - Start;
    const float Length = Delta.Size();
    const float Angle = FMath::RadiansToDegrees(
        FMath::Atan2(Delta.Y, Delta.X));

    if (UCanvasPanelSlot* ThreadSlot =
        Cast<UCanvasPanelSlot>(ThreadImage->Slot))
    {
        ThreadSlot->SetPosition((Start + End) * 0.5f);
        ThreadSlot->SetSize(FVector2D(Length, 40));
    }

    ThreadImage->SetRenderTransformAngle(Angle);
}
