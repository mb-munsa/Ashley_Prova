// Ashley


#include "Widgets/Ashley_SkipCutscene.h"
#include "Components/RadialSlider.h"

#include "DebugHelper.h"

void UAshley_SkipCutscene::UpdatePercentage(float Percentage)
{
	Slider->SetValue(Percentage);
}
