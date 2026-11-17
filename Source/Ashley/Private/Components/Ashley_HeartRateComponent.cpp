// Ashley


#include "Components/Ashley_HeartRateComponent.h"
#include "GameFramework/Character.h"
#include "Components/AudioComponent.h"
#include "Widgets/Ashley_HeartRateWidget.h"
#include "Widgets/Ashley_DamageVignette.h"
#include "Characters/Players/Ashley_FarmerPlayer.h"
#include "Characters/Players/Ashley_CharacterPlayer.h"

#include "DebugHelper.h"

// Sets default values for this component's properties
UAshley_HeartRateComponent::UAshley_HeartRateComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	NormalHeartRate = 0.f;
	MaxHeartRate = 100.f;
	HeartRateDeacceleration = 1.f;
	ScareHeartRateAcceleration = 1.f;
	RunHeartRateAcceleration = 1.f;
	SpeedMinToAugmentHeartRate = 310.f;
}


void UAshley_HeartRateComponent::HeartAttack()
{
	Debug::Print(TEXT("Heart Attack!!"));

	auto Farmer = Cast<AAshley_FarmerPlayer>(GetOwner());
	Farmer->FarmerHeartAttack();
}

void UAshley_HeartRateComponent::ResetHeartRate()
{
	HeartRate = NormalHeartRate;
	AudioComponentHeartbeat->SetSound(HeartBeatSlow);
	AudioComponentBreathing->SetSound(BreathingSound);

	AudioComponentHeartbeat->Play();
	AudioComponentBreathing->Play();

	AudioComponentHeartbeat->SetVolumeMultiplier(0);
	AudioComponentBreathing->SetVolumeMultiplier(0);
}

// Called when the game starts
void UAshley_HeartRateComponent::BeginPlay()
{
	Super::BeginPlay();

	bIsRunning = false;
	bIsScared = false;
	bIsRecuperating = false;

	HeartRate = NormalHeartRate;

	HeartWidget = CreateWidget<UAshley_HeartRateWidget>(GetWorld(), BP_HeartWidget);
	HeartWidget->AddToViewport(600);

	VignetteWidget = CreateWidget<UAshley_DamageVignette>(GetWorld(), BP_DamageVignette);
	VignetteWidget->AddToViewport(99);
	VignetteWidget->SetVisibility(ESlateVisibility::Collapsed);

	AAshley_CharacterPlayer* OwnerCharacter = Cast<AAshley_CharacterPlayer>(GetOwner());
	SpeedMinToAugmentHeartRate = OwnerCharacter->MaxVelocityWalk;
}

// Called every frame
void UAshley_HeartRateComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (bIsRunning || bIsScared || bIsRecuperating)
	{
		UpdateHeartRate(DeltaTime);
	}

	// TODO: temp visualization
	float HeartRatePerc = (HeartRate - NormalHeartRate) / (MaxHeartRate - NormalHeartRate);
	HeartWidget->SetProgress(HeartRatePerc, DeltaTime);
}


void UAshley_HeartRateComponent::UpdateHeartRate(float DeltaTime)
{
	float HeartRateChangeSpeed;

	UpdateHeartbeatAudio();

	if (HeartRate >= MaxHeartRate)
	{
		HeartAttack();
		return;
	}

	if (bIsRunning)
	{
		AAshley_CharacterPlayer* OwnerCharacter = Cast<AAshley_CharacterPlayer>(GetOwner());

		if (OwnerCharacter)
		{
			float Speed = OwnerCharacter->GetVelocity().Size();

			if (Speed > SpeedMinToAugmentHeartRate)
			{
				HeartRateChangeSpeed = RunHeartRateAcceleration; 

				if (bIsScared) HeartRateChangeSpeed += ScareHeartRateAcceleration;

				HeartRate = FMath::FInterpConstantTo(HeartRate, MaxHeartRate, DeltaTime, HeartRateChangeSpeed);
				return;
			}
		}
	}

	if (bIsScared)
	{
		HeartRateChangeSpeed = ScareHeartRateAcceleration;

		// Faccio pulsare l'indicatore di danno
		float PulseValue = FMath::Sin(GetWorld()->GetTimeSeconds() * 4.0f);
		float Intensity = FMath::GetMappedRangeValueClamped(
			FVector2D(-1.f, 1.f),
			FVector2D(0.25f, 0.55f),
			PulseValue
		);

		VignetteWidget->SetOpacity(Intensity);

		HeartRate = FMath::FInterpConstantTo(HeartRate, MaxHeartRate, DeltaTime, HeartRateChangeSpeed);
		return;
	}

	if (bIsRecuperating)
	{
		HeartRateChangeSpeed = HeartRateDeacceleration;
		HeartRate = FMath::FInterpConstantTo(HeartRate, NormalHeartRate, DeltaTime, HeartRateChangeSpeed);
		IsRecuperating();
	}
}

void UAshley_HeartRateComponent::UpdateHeartbeatAudio()
{
	int NewHeartLevel = HeartRate / 25;

	///Debug::Print(FString::Printf(TEXT("CurrentHeartLvl %d"), NewHeartLevel));

	if (NewHeartLevel != CurrentHeartLevel)
	{
		CurrentHeartLevel = NewHeartLevel;

		switch (CurrentHeartLevel)
		{
		case 1:
			AudioComponentHeartbeat->SetSound(HeartBeatSlow);
			break;
		case 2:
			AudioComponentHeartbeat->SetSound(HeartBeatFast);
			break;
		}
	}

	AudioComponentHeartbeat->SetVolumeMultiplier(HeartRate * 0.02f);
	AudioComponentBreathing->SetVolumeMultiplier((HeartRate * 0.02f) - 1);
	AudioComponentHeartbeat->SetPitchMultiplier(1 + (HeartRate * 0.01f));
}

void UAshley_HeartRateComponent::IsRecuperating()
{
	if (bIsScared) return;

	if (bIsRunning)
	{
		ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner());

		if (OwnerCharacter)
		{
			float Speed = OwnerCharacter->GetVelocity().Size();

			if (Speed < SpeedMinToAugmentHeartRate && !FMath::IsNearlyEqual(HeartRate, NormalHeartRate, .1f))
			{
				bIsRecuperating = true;
			}
			else bIsRecuperating = false;
		}
		return;
	}
		
	if (FMath::IsNearlyEqual(HeartRate, NormalHeartRate, .1f))
	{
		bIsRecuperating = false;
		HeartRate = NormalHeartRate;
	}
	else
	{
		bIsRecuperating = true;
	}
}

void UAshley_HeartRateComponent::StartRunning(bool bRunning)
{
	bIsRunning = bRunning;
	IsRecuperating();
}

void UAshley_HeartRateComponent::TriggerScare(bool bScare)
{
	bIsScared = bScare;
	IsRecuperating();
	ToggleVignette(bIsScared);
}

void UAshley_HeartRateComponent::ToggleVignette(bool bActivateVignette)
{
	if (bActivateVignette)
	{
		// TODO: aggiungere FadeIn - FadeOut
		VignetteWidget->SetVisibility(ESlateVisibility::Visible);
	}
	else VignetteWidget->SetVisibility(ESlateVisibility::Collapsed);
}

