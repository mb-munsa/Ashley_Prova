// Ashley

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InvestigationBoard/Ashley_InvestigationConnection.h"
#include "Ashley_InvestigationBoardTabWidget.h"

#include "Ashley_InvestigationBoardWidget.generated.h"

struct FInvestigationPostIt;
class UAshley_InvestigationPostItWidget;
class UAshley_InvestigationThreadWidget;
class UAshley_InvestigationDescriptionWidget;
class UInvestigationBoardDefinition;

class UCanvasPanel;
class UPanelWidget;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(
	FOnPostItsConnected, FName, FirstPostItId, FName, SecondPostItId);

/**
 * 
 */
UCLASS()
class ASHLEY_API UAshley_InvestigationBoardWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	void AddPostId(const FInvestigationPostIt& PostItId, bool bAnimate = false);

	// Queued: the thread is drawn as soon as both pins have a valid layout
	void AddConnection(FName PostItA, FName PostItB);

	// Removes every post-it and thread of the current board
	void ClearBoard();

	// Resets pan and zoom
	void ResetView();

	// Rebuilds the board tabs, one per board
	void SetBoards(const TArray<UInvestigationBoardDefinition*>& Boards, FName ActiveBoardId);

	void SetActiveTab(FName BoardId);

	UPROPERTY(BlueprintAssignable, Category = "Data")
	FOnBoardTabClicked OnBoardSelected;

	UPROPERTY(EditAnywhere, Category = "Board|Navigation")
	FVector2D MinBoardTranslation = FVector2D(-1000.f, -500.f);

	UPROPERTY(EditAnywhere, Category = "Board|Navigation")
	FVector2D MaxBoardTranslation = FVector2D(1000.f, 500.f);

	UPROPERTY(BlueprintAssignable, Category = "Data")
	FOnPostItsConnected OnPostItsConnected;

	void BeginThreadDrag(UAshley_InvestigationPostItWidget* Source);
	void UpdateThreadDrag(FVector2D ScreenPosition);

	// Shows the description next to the post-it, or hides it if it's already shown for that post-it
	void TogglePostItDescription(UAshley_InvestigationPostItWidget* PostIt);

	void HideDescription();

protected:

	// Draw order inside BoardCanvas
	static constexpr int32 PostItZOrder = 0;
	static constexpr int32 ThreadZOrder = 1;
	static constexpr int32 DescriptionZOrder = 10;

	UAshley_InvestigationPostItWidget* FindPostItPinAt(FVector2D ScreenPosition);

	enum class EDrawConnectionResult : uint8
	{
		Drawn,
		NotReady,
		Invalid
	};

	EDrawConnectionResult TryDrawConnection(const FInvestigationConnection& Connection);

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	UFUNCTION()
	void HandleTabClicked(FName BoardId);

	void CancelThreadDrag();

	virtual FReply NativeOnMouseMove(
		const FGeometry& Geometry,
		const FPointerEvent& Event) override;

	virtual FReply NativeOnMouseButtonUp(
		const FGeometry& Geometry,
		const FPointerEvent& Event) override;

	virtual FReply NativeOnMouseButtonDown(
		const FGeometry& Geometry,
		const FPointerEvent& Event) override;

	virtual FReply NativeOnMouseWheel(
		const FGeometry& Geometry,
		const FPointerEvent& Event) override;

	void ApplyBoardTransform();

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Data")
	TSubclassOf<UAshley_InvestigationPostItWidget> PostItWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "Data")
	TSubclassOf<UAshley_InvestigationThreadWidget> ThreadWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "Data")
	TSubclassOf<UAshley_InvestigationBoardTabWidget> TabWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "Data")
	TSubclassOf<UAshley_InvestigationDescriptionWidget> DescriptionWidgetClass;

	// Distance between the post-it's right edge and the description
	UPROPERTY(EditAnywhere, Category = "Data|Description")
	FVector2D DescriptionOffset = FVector2D(16.f, 0.f);

	// Tint of the threads connected to a deduction post-it
	UPROPERTY(EditAnywhere, Category = "Data|Threads")
	FLinearColor DeductionThreadTint = FLinearColor(0.35f, 0.6f, 1.0f, 1.0f);

	//UPROPERTY(EditAnywhere, Category = "Data|Threads")
	//FLinearColor DefaultColor = FLinearColor(0.35f, 0.6f, 1.0f, 1.0f);

	UPROPERTY(EditAnywhere, Category = "Data|Navigation")
	float ZoomSpeed = 1.15f;

	UPROPERTY(EditAnywhere, Category = "Data|Navigation")
	float MinZoom = 0.5f;

	UPROPERTY(EditAnywhere, Category = "Data|Navigation")
	float MaxZoom = 2.5f;

	float BoardZoom = 1.0f;
	FVector2D BoardTranslation = FVector2D::ZeroVector;
	FVector2D LastMousePosition = FVector2D::ZeroVector;
	bool bPanning = false;

	UPROPERTY(meta = (BindWidget))
	UCanvasPanel* BoardCanvas;

	// Container for the board tabs (e.g. a HorizontalBox)
	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UPanelWidget> TabsContainer;

	UPROPERTY()
	TObjectPtr<UAshley_InvestigationThreadWidget> ActiveThread;

	UPROPERTY()
	TObjectPtr<UAshley_InvestigationPostItWidget> DragSource;

	UPROPERTY()
	TArray<TObjectPtr<UAshley_InvestigationPostItWidget>> PostItWidgets;

	UPROPERTY()
	TMap<FName, TObjectPtr<UAshley_InvestigationPostItWidget>> PostItWidgetsMap;

	UPROPERTY()
	TArray<TObjectPtr<UAshley_InvestigationThreadWidget>> ThreadWidgets;

	UPROPERTY()
	TMap<FName, TObjectPtr<UAshley_InvestigationBoardTabWidget>> TabWidgets;

	// Single instance reused for every post-it, so at most one description is shown
	UPROPERTY()
	TObjectPtr<UAshley_InvestigationDescriptionWidget> DescriptionWidget;

	TArray<FInvestigationConnection> PendingConnections;
};
