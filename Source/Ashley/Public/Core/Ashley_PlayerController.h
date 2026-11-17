// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Core/Ashley_Checkpoint.h"
#include "Tags_Enums_Structs/Ashley_GamePlayTags.h"

#include "Ashley_PlayerController.generated.h"

class UAshley_InputConfigDA;
class AAshley_FixedCameraController;
class UAshley_CameraManager;
class AAshley_CharacterPlayer;
class UInputMappingContext;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInputClicked, FGameplayTag, InputAction);

UCLASS()
class ASHLEY_API AAshley_PlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	AAshley_PlayerController();

	UPROPERTY(BlueprintAssignable)
	FOnInputClicked OnInputClicked;

	UFUNCTION(BlueprintCallable)
	UAshley_CameraManager* GetCameraManager();

	UFUNCTION(BlueprintCallable)
	void ToggleInputBlock(bool bShouldBlockInput);

	UFUNCTION(BlueprintCallable)
	void ToggleSprintBlock(bool bShouldBlockSprint);

	UFUNCTION(BlueprintCallable)
	void ToggleHidePlayer(bool bShouldHidePlayer);

	UFUNCTION(BlueprintCallable)
	void TriggerPlayerReset();

	UFUNCTION(BlueprintCallable)
	void RegisterCheckpoint(UCheckpointData* SaveData);

	// InputMappings

	UFUNCTION(BlueprintCallable)
	void SetMappingContextByName(EAshley_InputMappingContext MappingContextName);

	UFUNCTION(BlueprintCallable)
	void AddMappingContextByName(EAshley_InputMappingContext MappingContextName, int Priority = 10);

	UFUNCTION(BlueprintCallable)
	void RemoveMappingContextByName(EAshley_InputMappingContext MappingContextName);

	UFUNCTION(BlueprintCallable)
	bool IsMousePressed() const { return bMousePressed; }

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AshleyPlayer")
	UAshley_CameraManager* CameraManager;

	// Usato per bloccare tutti gli input di movimento/interazione
	bool bInputBlocked;

	// Tasto sx mouse o equivalente joystick
	bool bMousePressed = false;

	// Blocca la possibilità di sprintare
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AshleyPlayer")
	bool bIsSprintBlocked;

	float PressStartTime = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AshleyPlayer")
	float SequenceSkipTimer = 1.0f;

private:
#pragma region Inputs
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "InputData", meta = (AllowPrivateAccess = "true"))
	UAshley_InputConfigDA* InputConfigDataAsset;

	UPROPERTY()
	UInputMappingContext* CurrentInputMapping;

	UPROPERTY()
	UCheckpointData* CurrentCheckpoint;

	UFUNCTION()
	void Input_Move(const FInputActionValue& InputActionValue);

	//UFUNCTION()
	//void Ashley_Input_Look(const FInputActionValue& InputActionValue);

	UFUNCTION()
	void Input_Jump();

	UFUNCTION()
	void Input_Esc();

	UFUNCTION()
	void Input_Inventory();

	UFUNCTION()
	void Input_ExitPuzzle();

	UFUNCTION()
	void Input_StopExaminingObject();

	UFUNCTION()
	void Input_RotateShowcase(const FInputActionValue& InputActionValue);

	UFUNCTION()
	void Input_SkipDialogue();

	UFUNCTION()
	void Input_Sprint_Stop(const FInputActionValue& InputActionValue);

	UFUNCTION()
	void Input_Sprint_Start(const FInputActionValue& InputActionValue);

	UFUNCTION()
	void Input_SkipCutscene_Ongoing(const FInputActionValue& InputActionValue);

	UFUNCTION()
	void Input_SkipCutscene_Started(const FInputActionValue& InputActionValue);

	UFUNCTION()
	void Input_SkipCutscene_Canceled(const FInputActionValue& InputActionValue);

	UFUNCTION()
	void Input_SkipCutscene_Completed(const FInputActionValue& InputActionValue);

	UFUNCTION()
	void Input_Mouse_Pressed(const FInputActionValue& InputActionValue);

	UFUNCTION()
	void Input_Mouse_Released(const FInputActionValue& InputActionValue);

	UFUNCTION()
	void Input_Interact();

	UFUNCTION()
	void Input_DropItem();

	// Attempts to interact with the Actor under the mouse cursor.
	UFUNCTION(BlueprintCallable, Category = "Interaction")
	void TryMouseInteract();

#pragma endregion 

	UPROPERTY()
	APawn* ControlledPawn;

	UPROPERTY()
	AAshley_CharacterPlayer* MyCharacter;

	UPROPERTY()
	FVector CurrentForward;

	UPROPERTY()
	FVector CurrentRight;

	UPROPERTY()
	FVector2D LastMovementVector;

	UPROPERTY()
	bool bIsSpringting;
};
