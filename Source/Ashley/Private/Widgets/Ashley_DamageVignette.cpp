// Ashley


#include "Widgets/Ashley_DamageVignette.h"

#include "Components/Image.h"

void UAshley_DamageVignette::NativeConstruct()
{
	Super::NativeConstruct();

	VignetteMID = UMaterialInstanceDynamic::Create(VignetteMaterial, this);
	VignetteImage->SetBrushFromMaterial(VignetteMID);
	VignetteMID->SetScalarParameterValue(TEXT("Opacity"), 1);
}

void UAshley_DamageVignette::SetIntensity(float NewIntensity)
{
	VignetteMID->SetScalarParameterValue(TEXT("Intensity"), NewIntensity);
}

void UAshley_DamageVignette::SetOpacity(float NewOpacity)
{
	VignetteImage->SetRenderOpacity(NewOpacity);
	//VignetteMID->SetScalarParameterValue(TEXT("Opacity"), NewOpacity);
}

void UAshley_DamageVignette::SetColor(FLinearColor NewColor)
{
	VignetteMID->SetVectorParameterValue(TEXT("Color"), NewColor);
}