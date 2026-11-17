// Ashley

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Ashley_HeartRateComponent.generated.h"

class UAshley_HeartRateWidget;
class UAshley_DamageVignette;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ASHLEY_API UAshley_HeartRateComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UAshley_HeartRateComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, Category = "HeartRate")
	void StartRunning(bool bRunning);

	UFUNCTION(BlueprintCallable, Category = "HeartRate")
	void TriggerScare(bool bScare);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data|Heart")
	TSubclassOf<UAshley_HeartRateWidget> BP_HeartWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data|Heart")
	TSubclassOf<UAshley_DamageVignette> BP_DamageVignette;

	UFUNCTION()
	void HeartAttack();

	UFUNCTION()
	void ResetHeartRate();

	// Audio component
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Data|Heart")
	UAudioComponent* AudioComponentHeartbeat;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Data|Heart")
	UAudioComponent* AudioComponentBreathing;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data|Heart")
	float NormalHeartRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data|Heart")
	float MaxHeartRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data|Heart")
	float RunHeartRateAcceleration;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data|Heart")
	float ScareHeartRateAcceleration;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data|Heart")
	float HeartRateDeacceleration;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Data|Heart")
	float HeartRate;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Data|Heart")
	float SpeedMinToAugmentHeartRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data|Heart")
	USoundBase* HeartBeatSlow;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data|Heart")
	USoundBase* HeartBeatFast;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data|Heart")
	USoundBase* BreathingSound;

private:

	UPROPERTY()
	UAshley_HeartRateWidget* HeartWidget;

	UPROPERTY()
	UAshley_DamageVignette* VignetteWidget;

	UFUNCTION()
	void UpdateHeartRate(float DeltaTime);

	UFUNCTION()
	void UpdateHeartbeatAudio();

	UFUNCTION()
	void IsRecuperating();

	UFUNCTION()
	void ToggleVignette(bool bActivateVignette);

	UPROPERTY()
	bool bIsRunning;

	UPROPERTY()
	bool bIsScared;

	UPROPERTY()
	bool bIsRecuperating;

	int CurrentHeartLevel = 0;

};
