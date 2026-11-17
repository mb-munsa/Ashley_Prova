// Ashley


#include "DataAssets/Ashley_PoltergeistDA.h"

uint8 UAshley_PoltergeistDA::GetActivationLevel() const
{
	return ActivationLevel;
}

EPoltergeistFrequency UAshley_PoltergeistDA::GetFrequency() const
{
	return Frequency;
}

EPoltergeistActivationType UAshley_PoltergeistDA::GetActivationType() const
{
	return ActivationType;
}

double UAshley_PoltergeistDA::GetCooldown() const
{
	return Cooldown;
}
