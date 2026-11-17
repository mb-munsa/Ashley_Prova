// Ashley


#include "InvestigationBoard/UI/Ashley_InvestigationBoardWidget.h"
#include <InvestigationBoard/UI/Ashley_InvestigationPostItWidget.h>
#include <InvestigationBoard/UI/Ashley_InvestigationThreadWidget.h>
#include <InvestigationBoard/UI/Ashley_InvestigationDescriptionWidget.h>

#include <InvestigationBoard/Ashley_InvestigationPostIt.h>
#include <InvestigationBoard/Ashley_InvestigationBoardDefinition.h>

#include "Components/CanvasPanelSlot.h"
#include "Components/CanvasPanel.h"
#include "Components/PanelWidget.h"
#include "InputCoreTypes.h"

#include "DebugHelper.h"

void UAshley_InvestigationBoardWidget::AddPostId(const FInvestigationPostIt& PostIt, bool bAnimate)
{
    if (!BoardCanvas || !PostItWidgetClass)
    {
        return;
    }

    UAshley_InvestigationPostItWidget* PostItWidget =
        CreateWidget<UAshley_InvestigationPostItWidget>(GetOwningPlayer(), PostItWidgetClass);

    if (!PostItWidget)
    {
        return;
    }

    PostItWidget->SetPostIt(PostIt, this);

    UCanvasPanelSlot* PostItSlot = BoardCanvas->AddChildToCanvas(PostItWidget);
    PostItSlot->SetPosition(PostIt.DefaultPosition);
    PostItSlot->SetAutoSize(true);
    PostItSlot->SetZOrder(PostItZOrder);

    PostItWidgets.Add(PostItWidget);
    PostItWidgetsMap.Add(PostIt.Id, PostItWidget);

    if (bAnimate)
        PostItWidget->PlayRevealAnimation();
}

void UAshley_InvestigationBoardWidget::AddConnection(FName PostItA, FName PostItB)
{
    // The pins have no geometry until Slate lays them out, so the thread is drawn in NativeTick
    PendingConnections.Add(FInvestigationConnection(PostItA, PostItB));
}

void UAshley_InvestigationBoardWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
    Super::NativeTick(MyGeometry, InDeltaTime);

    for (int32 Index = PendingConnections.Num() - 1; Index >= 0; --Index)
    {
        if (TryDrawConnection(PendingConnections[Index]) != EDrawConnectionResult::NotReady)
            PendingConnections.RemoveAt(Index);
    }
}

UAshley_InvestigationBoardWidget::EDrawConnectionResult
UAshley_InvestigationBoardWidget::TryDrawConnection(const FInvestigationConnection& Connection)
{
    if (!BoardCanvas || !ThreadWidgetClass)
    {
        return EDrawConnectionResult::Invalid;
    }

    // Ricavo i widget corrispondenti
    const TObjectPtr<UAshley_InvestigationPostItWidget>* PostItAWidget = PostItWidgetsMap.Find(Connection.A);
    const TObjectPtr<UAshley_InvestigationPostItWidget>* PostItBWidget = PostItWidgetsMap.Find(Connection.B);

    if (!PostItAWidget || !*PostItAWidget || !PostItBWidget || !*PostItBWidget)
    {
        return EDrawConnectionResult::Invalid;
    }

    const FGeometry StartPinGeometry = (*PostItAWidget)->GetPinGeometry();
    const FGeometry EndPinGeometry = (*PostItBWidget)->GetPinGeometry();

    if (StartPinGeometry.GetLocalSize().IsNearlyZero() || EndPinGeometry.GetLocalSize().IsNearlyZero())
    {
        return EDrawConnectionResult::NotReady;
    }

    auto ThreadWidget = CreateWidget<UAshley_InvestigationThreadWidget>(
        GetOwningPlayer(), ThreadWidgetClass);

    if (!ThreadWidget)
    {
        return EDrawConnectionResult::Invalid; 
    }

    BoardCanvas->AddChildToCanvas(ThreadWidget)->SetZOrder(ThreadZOrder);
    ThreadWidget->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

    if ((*PostItAWidget)->PostItData.bRequireDeduction || (*PostItBWidget)->PostItData.bRequireDeduction)
        ThreadWidget->SetTint(DeductionThreadTint);
    ThreadWidgets.Add(ThreadWidget);

    const FVector2D StartScreen = StartPinGeometry.LocalToAbsolute(
        StartPinGeometry.GetLocalSize() * 0.5f
    );
    const FVector2D EndScreen = EndPinGeometry.LocalToAbsolute(
        EndPinGeometry.GetLocalSize() * 0.5f
    );

    const FGeometry CanvasGeometry = BoardCanvas->GetCachedGeometry();

    const FVector2D Start = CanvasGeometry.AbsoluteToLocal(StartScreen);
    const FVector2D End = CanvasGeometry.AbsoluteToLocal(EndScreen);

    ThreadWidget->SetEndpoints(Start, End);

    return EDrawConnectionResult::Drawn;
}

void UAshley_InvestigationBoardWidget::ClearBoard()
{
    CancelThreadDrag();
    HideDescription();

    for (UAshley_InvestigationThreadWidget* Thread : ThreadWidgets)
    {
        if (Thread)
            Thread->RemoveFromParent();
    }

    for (UAshley_InvestigationPostItWidget* PostIt : PostItWidgets)
    {
        if (PostIt)
            PostIt->RemoveFromParent();
    }

    ThreadWidgets.Empty();
    PostItWidgets.Empty();
    PostItWidgetsMap.Empty();
    PendingConnections.Empty();
}

// Description

void UAshley_InvestigationBoardWidget::TogglePostItDescription(UAshley_InvestigationPostItWidget* PostIt)
{
    if (!PostIt || !BoardCanvas || !DescriptionWidgetClass)
        return;

    // Same post-it clicked again: close it
    if (DescriptionWidget &&
        DescriptionWidget->GetVisibility() != ESlateVisibility::Collapsed &&
        DescriptionWidget->GetPostItId() == PostIt->GetPostItId())
    {
        HideDescription();
        return;
    }

    if (!DescriptionWidget)
    {
        DescriptionWidget = CreateWidget<UAshley_InvestigationDescriptionWidget>(
            GetOwningPlayer(), DescriptionWidgetClass);

        if (!DescriptionWidget)
            return;

        UCanvasPanelSlot* DescriptionSlot = BoardCanvas->AddChildToCanvas(DescriptionWidget);
        DescriptionSlot->SetAutoSize(true);
        DescriptionSlot->SetZOrder(DescriptionZOrder);
    }

    DescriptionWidget->SetPostIt(PostIt->PostItData);

    // Next to the post-it's right edge, in canvas space
    if (const UCanvasPanelSlot* PostItSlot = Cast<UCanvasPanelSlot>(PostIt->Slot))
    {
        if (UCanvasPanelSlot* DescriptionSlot = Cast<UCanvasPanelSlot>(DescriptionWidget->Slot))
        {
            DescriptionSlot->SetPosition(PostItSlot->GetPosition()
                + FVector2D(PostIt->GetDesiredSize().X, 0.f)
                + DescriptionOffset);
        }
    }

    DescriptionWidget->SetVisibility(ESlateVisibility::Visible);
}

void UAshley_InvestigationBoardWidget::HideDescription()
{
    if (DescriptionWidget)
        DescriptionWidget->SetVisibility(ESlateVisibility::Collapsed);
}

void UAshley_InvestigationBoardWidget::ResetView()
{
    BoardZoom = 1.0f;
    BoardTranslation = FVector2D::ZeroVector;
    bPanning = false;

    ApplyBoardTransform();
}

// Board tabs

void UAshley_InvestigationBoardWidget::SetBoards(
    const TArray<UInvestigationBoardDefinition*>& Boards, FName ActiveBoardId)
{
    if (!TabsContainer || !TabWidgetClass)
        return;

    TabsContainer->ClearChildren();
    TabWidgets.Empty();

    for (const UInvestigationBoardDefinition* Board : Boards)
    {
        if (!Board)
            continue;

        UAshley_InvestigationBoardTabWidget* Tab =
            CreateWidget<UAshley_InvestigationBoardTabWidget>(GetOwningPlayer(), TabWidgetClass);

        if (!Tab)
            continue;

        Tab->Setup(Board);
        Tab->OnTabClicked.AddDynamic(this, &ThisClass::HandleTabClicked);

        TabsContainer->AddChild(Tab);
        TabWidgets.Add(Tab->GetBoardId(), Tab);
    }

    // With a single board there is nothing to switch to
    TabsContainer->SetVisibility(TabWidgets.Num() > 1
        ? ESlateVisibility::SelfHitTestInvisible
        : ESlateVisibility::Collapsed);

    SetActiveTab(ActiveBoardId);
}

void UAshley_InvestigationBoardWidget::SetActiveTab(FName BoardId)
{
    for (const auto& Pair : TabWidgets)
    {
        if (Pair.Value)
            Pair.Value->SetSelected(Pair.Key == BoardId);
    }
}

void UAshley_InvestigationBoardWidget::HandleTabClicked(FName BoardId)
{
    OnBoardSelected.Broadcast(BoardId);
}

// Thread movement

void UAshley_InvestigationBoardWidget::BeginThreadDrag(
    UAshley_InvestigationPostItWidget* Source)
{
    if (!Source || !ThreadWidgetClass)
        return;

    HideDescription();

    DragSource = Source;

    ActiveThread = CreateWidget<UAshley_InvestigationThreadWidget>(
        GetOwningPlayer(), ThreadWidgetClass);

    if (!ActiveThread)
        return;

    ActiveThread->AddToViewport(510);
    ActiveThread->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

    UpdateThreadDrag(
        Source->GetPinGeometry().GetAbsolutePosition());
}

void UAshley_InvestigationBoardWidget::UpdateThreadDrag(
    FVector2D ScreenPosition)
{
    if (!ActiveThread || !DragSource)
        return;

    const FGeometry PinGeometry = DragSource->GetPinGeometry();

    const FVector2D StartScreen = PinGeometry.LocalToAbsolute(
        PinGeometry.GetLocalSize() * 0.5f
    );

    const FGeometry ViewportGeometry = GetCachedGeometry();

    const FVector2D Start = ViewportGeometry.AbsoluteToLocal(StartScreen);
    const FVector2D End = ViewportGeometry.AbsoluteToLocal(ScreenPosition);

    ActiveThread->SetEndpoints(Start, End);
}

UAshley_InvestigationPostItWidget*
UAshley_InvestigationBoardWidget::FindPostItPinAt(
    FVector2D ScreenPosition)
{
    for (UAshley_InvestigationPostItWidget* PostIt : PostItWidgets)
    {
        if (PostIt &&
            PostIt->GetPinGeometry().IsUnderLocation(ScreenPosition))
        {
            return PostIt;
        }
    }

    return nullptr;
}

FReply UAshley_InvestigationBoardWidget::NativeOnMouseMove(
    const FGeometry& Geometry,
    const FPointerEvent& Event)
{
    // Dragging thread
    if (DragSource)
    {
        UpdateThreadDrag(Event.GetScreenSpacePosition());
        return FReply::Handled();
    }

    // Panning
    if (bPanning)
    {
        const FVector2D CurrentMouse =
            Event.GetScreenSpacePosition();

        const FVector2D PreviousLocal =
            Geometry.AbsoluteToLocal(LastMousePosition);

        const FVector2D CurrentLocal =
            Geometry.AbsoluteToLocal(CurrentMouse);

        BoardTranslation += CurrentLocal - PreviousLocal;

        BoardTranslation.X = FMath::Clamp(
            BoardTranslation.X,
            MinBoardTranslation.X,
            MaxBoardTranslation.X);

        BoardTranslation.Y = FMath::Clamp(
            BoardTranslation.Y,
            MinBoardTranslation.Y,
            MaxBoardTranslation.Y);

        LastMousePosition = CurrentMouse;

        ApplyBoardTransform();

        return FReply::Handled();
    }

    return Super::NativeOnMouseMove(Geometry, Event);
}

FReply UAshley_InvestigationBoardWidget::NativeOnMouseButtonUp(
    const FGeometry& Geometry,
    const FPointerEvent& Event)
{
    if (bPanning && Event.GetEffectingButton() == EKeys::RightMouseButton)
    {
        bPanning = false;

        return FReply::Handled().ReleaseMouseCapture();
    }

    if (!DragSource)
        return Super::NativeOnMouseButtonUp(Geometry, Event);

    UAshley_InvestigationPostItWidget* Source = DragSource;
    UAshley_InvestigationPostItWidget* Target =
        FindPostItPinAt(Event.GetScreenSpacePosition());

    if (Target && Target != Source)
    {
        Debug::Print("Connecting!");
        OnPostItsConnected.Broadcast(
            Source->GetPostItId(),
            Target->GetPostItId());
    }

    CancelThreadDrag();

    return FReply::Handled().ReleaseMouseCapture();
}

void UAshley_InvestigationBoardWidget::CancelThreadDrag()
{
    DragSource = nullptr;

    if (ActiveThread)
    {
        ActiveThread->RemoveFromParent();
        ActiveThread = nullptr;
    }
}

FReply UAshley_InvestigationBoardWidget::NativeOnMouseButtonDown(
    const FGeometry& Geometry,
    const FPointerEvent& Event)
{
    if (Event.GetEffectingButton() == EKeys::RightMouseButton)
    {
        bPanning = true;
        LastMousePosition = Event.GetScreenSpacePosition();

        return FReply::Handled().CaptureMouse(TakeWidget());
    }

    // Left click on empty board space closes the description
    // (clicks on post-its and on the description are handled before reaching here)
    if (Event.GetEffectingButton() == EKeys::LeftMouseButton)
        HideDescription();

    return Super::NativeOnMouseButtonDown(Geometry, Event);
}

FReply UAshley_InvestigationBoardWidget::NativeOnMouseWheel(
    const FGeometry& Geometry,
    const FPointerEvent& Event)
{
    if (!BoardCanvas)
        return Super::NativeOnMouseWheel(Geometry, Event);

    const float OldZoom = BoardZoom;

    BoardZoom = FMath::Clamp(
        BoardZoom * FMath::Pow(ZoomSpeed, Event.GetWheelDelta()),
        MinZoom,
        MaxZoom);

    if (FMath::IsNearlyEqual(OldZoom, BoardZoom))
        return FReply::Handled();

    // Cursor position in the board widget's parent coordinate space.
    const FVector2D CurrentCursor =
        Geometry.AbsoluteToLocal(Event.GetScreenSpacePosition());

    // Keep the point under the cursor fixed during zoom.
    BoardTranslation =
        CurrentCursor - (CurrentCursor - BoardTranslation) * (BoardZoom / OldZoom);

    ApplyBoardTransform();

    return FReply::Handled();
}

void UAshley_InvestigationBoardWidget::ApplyBoardTransform()
{
    if (!BoardCanvas)
        return;

    FWidgetTransform Transform;
    Transform.Translation = BoardTranslation;
    Transform.Scale = FVector2D(BoardZoom, BoardZoom);
    Transform.Angle = 0.0f;
    Transform.Shear = FVector2D::ZeroVector;

    BoardCanvas->SetRenderTransform(Transform);
}
