// Ashley

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "Ashley_InvestigationBoardTabWidget.generated.h"

class UInvestigationBoardDefinition;
class UButton;
class UTextBlock;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBoardTabClicked, FName, BoardId);

/**
 * Button that switches the investigation board to the board it represents.
 */
UCLASS()
class ASHLEY_API UAshley_InvestigationBoardTabWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	void Setup(const UInvestigationBoardDefinition* Board);

	void SetSelected(bool bSelected);

	FName GetBoardId() const;

	UPROPERTY(BlueprintAssignable, Category = "Data")
	FOnBoardTabClicked OnTabClicked;

protected:

	virtual void NativeOnInitialized() override;

	UFUNCTION()
	void HandleClicked();

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> TabButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TabLabel;

	FName BoardId;
};
