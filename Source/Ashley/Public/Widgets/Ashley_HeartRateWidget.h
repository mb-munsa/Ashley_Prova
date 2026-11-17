// Ashley

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/ProgressBar.h"

#include "Ashley_HeartRateWidget.generated.h"

class UImage;

/**
 * 
 */
UCLASS()
class ASHLEY_API UAshley_HeartRateWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	void NativePreConstruct() override;

	void SetProgress(float Progress, float DeltaTime = 0);

	UPROPERTY(meta = (BindWidget), BlueprintReadWrite)
	UImage* HeartRateImage;

protected:

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UMaterialInstanceDynamic> HeartRateMID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UMaterial> HeartRateMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FLinearColor ColorNormal;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FLinearColor ColorWarning;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FLinearColor ColorAlert;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SpeedNormal = 0.75f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SpeedMedium = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SpeedHigh = 1.25f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SpeedMax = 1.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BlendDuration = 0.8f;

	float CurrentSpeed = 0.75f;
	float BlendTime = 0;

	bool bIsBlendingColor = false;
	bool bIsCurrentlyVisible = false;
	FLinearColor CurrentColor;
	FLinearColor DestinationColor;
};
