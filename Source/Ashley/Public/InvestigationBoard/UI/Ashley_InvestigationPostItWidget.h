// Ashley

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InvestigationBoard/Ashley_InvestigationPostIt.h"

#include "Ashley_InvestigationPostItWidget.generated.h"

class UAshley_InvestigationBoardWidget;
class UVerticalBox;
class UImage;
class UButton;
class UTextBlock;


/**
 * 
 */
UCLASS()
class ASHLEY_API UAshley_InvestigationPostItWidget : public UUserWidget
{
	GENERATED_BODY()

protected:

    void NativeConstruct() override;

public:

	void SetPostIt(
        const FInvestigationPostIt& PostIdDefinition, 
        UAshley_InvestigationBoardWidget* BoardWidgetRef);

    FReply NativeOnMouseButtonDown(
        const FGeometry& Geometry,
        const FPointerEvent& Event) override;

    UFUNCTION()
    FGeometry GetPinGeometry() const;

    UFUNCTION()
    FName GetPostItId() const;


    UFUNCTION()
    void OnPostItClicked();

    // Played when the post-it is revealed while the board is already known (new clue)
    UFUNCTION(BlueprintImplementableEvent)
    void PlayRevealAnimation();

    UPROPERTY()
    UAshley_InvestigationBoardWidget* BoardWidget;

    UPROPERTY()
    FInvestigationPostIt PostItData;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UImage> PinImage;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UVerticalBox> ContentBox;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UImage> PostItImage;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UTextBlock> PostItTitle;

    bool bDraggingPin = false;
	
};
