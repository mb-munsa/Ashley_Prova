// Ashley


#include "Widgets/Ashley_HeartRateWidget.h"

#include "Components/Image.h"

#include "DebugHelper.h"

FLinearColor LerpColors(const FLinearColor& ColorA, const FLinearColor& ColorB, float Alpha)
{
	//FLinearColor LinearA = FLinearColor(MyFColor);
	//FLinearColor LinearB = FLinearColor(MyFColor);
	FLinearColor Result = FLinearColor::LerpUsingHSV(ColorA, ColorB, Alpha);
	Result.A = 1.f;
	return Result;
}

void UAshley_HeartRateWidget::NativePreConstruct()
{
	Super::NativePreConstruct();

	HeartRateMID = UMaterialInstanceDynamic::Create(HeartRateMaterial, this);
	HeartRateImage->SetBrushFromMaterial(HeartRateMID);

	CurrentColor = ColorNormal;
	CurrentSpeed = SpeedNormal;
	bIsCurrentlyVisible = false;
	HeartRateImage->SetVisibility(ESlateVisibility::Collapsed);
}

void UAshley_HeartRateWidget::SetProgress(float Progress, float DeltaTime)
{ 
	//auto CurrentTiling = FMath::GetMappedRangeValueClamped(FVector2D(0, 1), FVector2D(1, 1.5f), Progress);
	//Debug::Print(FString::Printf(TEXT("Curr speed: %f"), CurrentTiling));

	if (!bIsCurrentlyVisible)
	{
		if (Progress >= 0.1f)
		{
			bIsCurrentlyVisible = true;
			HeartRateImage->SetVisibility(ESlateVisibility::Visible);
		}
		else return;
	}
	else if (Progress <= 0.1f)
	{
		bIsCurrentlyVisible = false;
		HeartRateImage->SetVisibility(ESlateVisibility::Collapsed);
		return;
	}


	float NewSpeed = SpeedNormal;
	FLinearColor NewColor;

	if (Progress < 0.25f)
	{
		//NewSpeed = SpeedNormal;
		NewColor = ColorNormal;
	}
	else if (Progress > 0.25f && Progress < 0.50f)
	{
		//NewSpeed = SpeedMedium;
		NewColor = ColorWarning;
	}
	else if (Progress > 0.50f && Progress < 0.75f)
	{
		//NewSpeed = SpeedHigh;
		NewColor = ColorAlert;
	}
	else
	{
		//NewSpeed = SpeedMax;
		NewColor = ColorAlert;
	}

	//NewSpeed = FMath::GetMappedRangeValueClamped(FVector2D(0, 1), FVector2D(SpeedNormal, SpeedMax), Progress);
	//Debug::Print(FString::Printf(TEXT("Curr speed: %f"), NewSpeed));

	// Modifico Colore
	if (bIsBlendingColor)
	{
		//Debug::Print(FString::Printf(TEXT("Blending: %f"), BlendTime));
		BlendTime += DeltaTime;
		auto BlendedColor = LerpColors(CurrentColor, DestinationColor, BlendTime / BlendDuration);

		if (FMath::Abs(BlendTime - BlendDuration) < 0.1f)
		{
			//Debug::Print("End blending!");

			bIsBlendingColor = false;
			BlendedColor = NewColor;
			CurrentColor = NewColor;
		}

		HeartRateMID->SetVectorParameterValue(FName("_Color"), BlendedColor);
	}
	else if (NewColor != CurrentColor)
	{
		//Debug::Print("Start blending!");

		bIsBlendingColor = true;
		BlendTime = 0;
		DestinationColor = NewColor;
	}
	// Modifico Speed
	//if (NewSpeed != CurrentSpeed)
	//{
	//	HeartRateMID->SetScalarParameterValue(FName("_WaveSpeed"), NewSpeed);
	//	CurrentSpeed = NewSpeed;
	//}
	// Modifico U Tiling
	//HeartRateMID->SetScalarParameterValue(FName("_Tiling"), CurrentTiling);
}


