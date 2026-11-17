// Ashley

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "Ashley_InteractionPrompt.generated.h"

/**
 * 
 */
UCLASS()
class ASHLEY_API UAshley_InteractionPrompt : public UWidgetComponent
{
	GENERATED_BODY()

	//UAshley_InteractionPrompt();
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


public:

	void SetText(const FString& TextToDisplay) const;

	UFUNCTION(BlueprintCallable)
	void Show();

	UFUNCTION(BlueprintCallable)
	void Hide();


protected:

	bool bIsShowing = false;
	
};
