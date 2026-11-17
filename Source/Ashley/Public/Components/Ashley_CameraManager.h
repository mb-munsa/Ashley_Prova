// Ashley

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Ashley_CameraManager.generated.h"

class AAshley_FixedCameraController;
class AAshley_PlayerController;
class AAshley_CharacterPlayer;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ASHLEY_API UAshley_CameraManager : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UAshley_CameraManager();

	// Single access point: resolves the manager through the world's local player controller,
	// so it does not depend on the pawn being possessed (e.g. during level teardown).
	UFUNCTION(BlueprintPure, Category = "Camera", meta = (WorldContext = "WorldContextObject"))
	static UAshley_CameraManager* Get(const UObject* WorldContextObject);

	//UFUNCTION()
	//void SetPreviousCamera();
	UFUNCTION(BlueprintCallable)
	void SetCurrentCamera(AAshley_FixedCameraController* NewCamera);
	UFUNCTION(BlueprintCallable)
	void RemoveFromCameraStack(AAshley_FixedCameraController* CameraToRemove);
	UFUNCTION(BlueprintCallable)
	void AddToCameraStack(AAshley_FixedCameraController* CameraToAdd);

	UFUNCTION(BlueprintCallable)
	AAshley_FixedCameraController* GetCurrentCamera();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION()
	FVector GetCurrentForward() const;
	UFUNCTION()
	FVector GetCurrentRight() const;

	UFUNCTION(BlueprintCallable)
	void ActivateSoundOnCameraSwitch(bool bShouldActivate) { bShouldPlaySoundOnCameraSwitch = bShouldActivate; }

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	void FindAndAssignFirstCamera();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	USoundBase* CameraChangeSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	bool bShouldPlaySoundOnCameraSwitch = false;

	//UPROPERTY()
	//AAshley_FixedCameraController* FirstCamera;
	UPROPERTY()
	AAshley_FixedCameraController* CurrentCamera;
	//UPROPERTY()
	//AAshley_FixedCameraController* PreviousCamera;

	UPROPERTY(VisibleAnywhere, Category = "Data")
	TArray<AAshley_FixedCameraController*> CameraStack;

	UPROPERTY()
	FVector CurrentForwardVector;
	UPROPERTY()
	FVector CurrentRightVector;

	UPROPERTY()
	AAshley_PlayerController* PlayerControllerRef;

	UPROPERTY()
	AAshley_CharacterPlayer* CharacterRef;

	UPROPERTY()
	bool bIsFollowingPlayer;
};
