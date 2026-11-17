// Ashley

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Ashley_FixedCameraController.generated.h"

class UBoxComponent;
class UCineCameraComponent;
class UArrowComponent;

UCLASS()
class ASHLEY_API AAshley_FixedCameraController : public AActor
{
	GENERATED_BODY()
	
public:	
	AAshley_FixedCameraController();

	float GetBlendTime() const;

	FVector GetForwardDirection() const;
	FVector GetRightDirection() const;

	FVector GetCurrentForwardDirection() const;
	FVector GetCurrentRightDirection() const;

	bool GetShouldTrackPlayer() const;

	UFUNCTION()
	void FollowPlayer(AActor* ActorToFollow, float DeltaTime);

	UFUNCTION()
	void SnapToPlayer(AActor* PlayerChar);

	UFUNCTION()
	void ResetCamera();

	UFUNCTION()
	UCineCameraComponent* GetCameraComponent();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables")
	bool IsFirstCamera;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables")
	bool bShouldTrackPlayer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables")
	bool bDestroyOnExit = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables")
	UBoxComponent* TriggerVolume;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables")
	UCineCameraComponent* Camera;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables")
	float BlendTime = 0.0f;

	UPROPERTY()
	FRotator InitialRotation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables")
	float MaxDelta = 20;

	UPROPERTY(EditAnywhere, Category = "Variables")
	UArrowComponent* DirectionalArrow;

	// True if any of this camera's trigger boxes still overlaps the actor
	bool IsActorInsideTriggers(const AActor* Actor) const;

	UFUNCTION()
	void OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
};
