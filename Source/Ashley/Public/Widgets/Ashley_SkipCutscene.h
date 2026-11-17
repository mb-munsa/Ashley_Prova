// Ashley

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Ashley_SkipCutscene.generated.h"

class UTextBlock;
class URadialSlider;

UCLASS()
class ASHLEY_API UAshley_SkipCutscene : public UUserWidget
{
	GENERATED_BODY()
	

public:

	UFUNCTION()
	void UpdatePercentage(float Percentage);

protected:

	UPROPERTY(meta = (BindWidget))
	URadialSlider* Slider;
};
