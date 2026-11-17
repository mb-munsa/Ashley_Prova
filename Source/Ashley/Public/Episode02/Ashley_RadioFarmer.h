// Ashley

#pragma once

#include "CoreMinimal.h"
#include "Interaction/Ashley_ActorInteract.h"
#include "Core/Ashley_Poltergeist.h"
#include "Ashley_RadioFarmer.generated.h"

class UAudioComponent;
class AAshley_CharacterPlayer;

USTRUCT(BlueprintType)
struct FSubtitle
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	FString Subtitle;
	UPROPERTY(EditAnywhere)
	float TimestampStart = 0.0f;
	//UPROPERTY(EditAnywhere)
	//float TimestampEnd;
};

USTRUCT(BlueprintType)
struct FSubtitledSound
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	USoundBase* Sound = nullptr;

	UPROPERTY(EditAnywhere)
	TArray<FSubtitle> Subtitles;

	UPROPERTY(EditAnywhere)
	float SoundDuration = 5.0f;
};


/**
 * 
 */
UCLASS()
class ASHLEY_API AAshley_RadioFarmer : public AAshley_Poltergeist
{
	GENERATED_BODY()

public:

	// Sets default values for this actor's properties
	AAshley_RadioFarmer();

	virtual void Tick(float DeltaTime) override;
	virtual void Interact_Implementation() override;

	virtual FString GetInteractMessage() const override;

protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void OnBoxInteractOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
	void OnBoxInteractEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;

	UFUNCTION()
	void ShowDialogue(FString Dialogue);
	UFUNCTION()
	void HideDialogue();

	UFUNCTION(BlueprintCallable)
	void SwitchToHauntedChannel();

	UFUNCTION()
	FString CalculateCurrentSubtitle();

	// Audio component
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Data")
	TArray<UAudioComponent*> AudioComponentsRadio;

	UPROPERTY(BlueprintReadWrite, Category = "Data")
	UAudioComponent* AudioComponentSwitch;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	TArray<FSubtitledSound> RadioMessages;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	TArray<float> RadioChannelsPlayback;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	float MaxDistance = 1000.0f;
	
	UPROPERTY()
	int CurrentChannel;
	UPROPERTY()
	int ChannelsNumber;
	UPROPERTY()
	bool bIsShowingDialogue;

	UPROPERTY()
	float CurrentVolume;

	FString CurrentDialogue = "";

	AAshley_CharacterPlayer* Player;
};
