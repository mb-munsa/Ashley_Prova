// Ashley

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "Ashley_InvestigationDescriptionWidget.generated.h"

struct FInvestigationPostIt;
class UTextBlock;

/**
 * Description of the selected post-it, shown above post-its and threads.
 */
UCLASS()
class ASHLEY_API UAshley_InvestigationDescriptionWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	void SetPostIt(const FInvestigationPostIt& PostIt);

	FName GetPostItId() const;

protected:

	// Left clicks on the description don't reach the board (which would close it)
	virtual FReply NativeOnMouseButtonDown(
		const FGeometry& Geometry,
		const FPointerEvent& Event) override;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> DescriptionText;

	// Optional: repeat the title in the description panel
	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UTextBlock> TitleText;

	FName PostItId;
};
