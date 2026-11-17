// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/Ashley_PlayerController.h"

#include "EnhancedInputSubsystems.h"
#include "DataAssets/Ashley_InputConfigDA.h"
#include "Components/Ashley_InputComponent.h"
#include "Core/Ashley_FixedCameraController.h"
#include "Core/Ashley_TriggerCollider.h"
#include "Components/Ashley_CameraManager.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/Character.h"
#include "Characters/Players/Ashley_CharacterPlayer.h"
#include "Core/Ashley_DialogueComponent.h"

#include "DebugHelper.h"
#include "Managers/Ashley_SequenceManager.h"
#include <Managers/Ashley_WidgetSystem.h>
#include <Kismet/GameplayStatics.h>
#include <Interaction/Ashley_ClickableInterface.h>

AAshley_PlayerController::AAshley_PlayerController()
{
	CameraManager = CreateDefaultSubobject<UAshley_CameraManager>(TEXT("Camera_Manager"));
	//CameraManager->SetupAttachment();

	//WidgetManager = CreateDefaultSubobject<UAshley_WidgetManager>(TEXT("Widget_Manager"));
	//WidgetManager->RegisterComponent();
}

void AAshley_PlayerController::BeginPlay()
{
	Super::BeginPlay();

	checkf(InputConfigDataAsset, TEXT("Forgot to assign a valid data asset as input config"));

	if (ULocalPlayer* LocalPlayer = GetLocalPlayer())
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer))
		{
			Subsystem->AddMappingContext(InputConfigDataAsset->DefaultMappingContext, 0);
			CurrentInputMapping = InputConfigDataAsset->DefaultMappingContext;
		}
	}
	ControlledPawn = GetPawn();
	MyCharacter = Cast<AAshley_CharacterPlayer>(ControlledPawn);
	
	CurrentForward = FVector::ForwardVector;
	CurrentRight = FVector::RightVector;
	bIsSpringting = false;
	bInputBlocked = false;

	UGameplayStatics::GetGameInstance(this)->
		GetSubsystem<UAshley_WidgetSystem>()->InitializeHUD();
}

void AAshley_PlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UAshley_InputComponent* Ashley_InputComponent = CastChecked<UAshley_InputComponent>(InputComponent);

	Ashley_InputComponent->SetTickableWhenPaused(true);
	this->SetTickableWhenPaused(true);

	// Movimenti
	Ashley_InputComponent->BindNativeInputAction(InputConfigDataAsset, AshleyGameplayTags::Ashley_InputAction_Move, ETriggerEvent::Triggered, this, &ThisClass::Input_Move);
	Ashley_InputComponent->BindNativeInputAction(InputConfigDataAsset, AshleyGameplayTags::Ashley_InputAction_Move, ETriggerEvent::Completed, this, &ThisClass::Input_Move);
	
	Ashley_InputComponent->BindNativeInputAction(InputConfigDataAsset, AshleyGameplayTags::Ashley_InputAction_MousePress , ETriggerEvent::Started, this, &ThisClass::Input_Mouse_Pressed);
	Ashley_InputComponent->BindNativeInputAction(InputConfigDataAsset, AshleyGameplayTags::Ashley_InputAction_MousePress, ETriggerEvent::Completed, this, &ThisClass::Input_Mouse_Released);

	//Ashley_InputComponent->BindNativeInputAction(InputConfigDataAsset, AshleyGameplayTags::Ashley_InputAction_Look, ETriggerEvent::Triggered, this, &ThisClass::Ashley_Input_Look);
	//Ashley_InputComponent->BindNativeInputAction(InputConfigDataAsset, AshleyGameplayTags::Ashley_InputAction_Jump, ETriggerEvent::Triggered, this, &ThisClass::Ashley_Input_Jump);
	Ashley_InputComponent->BindNativeInputAction(InputConfigDataAsset, AshleyGameplayTags::Ashley_InputAction_Interact, ETriggerEvent::Triggered, this, &ThisClass::Input_Interact);
	Ashley_InputComponent->BindNativeInputAction(InputConfigDataAsset, AshleyGameplayTags::Ashley_InputAction_Run, ETriggerEvent::Triggered, this, &ThisClass::Input_Sprint_Start);
	Ashley_InputComponent->BindNativeInputAction(InputConfigDataAsset, AshleyGameplayTags::Ashley_InputAction_Run, ETriggerEvent::Completed, this, &ThisClass::Input_Sprint_Stop);
	Ashley_InputComponent->BindNativeInputAction(InputConfigDataAsset, AshleyGameplayTags::Ashley_InputAction_Drop, ETriggerEvent::Triggered, this, &ThisClass::Input_DropItem);

	// UI
	Ashley_InputComponent->BindNativeInputAction(InputConfigDataAsset, AshleyGameplayTags::Ashley_InputAction_StopExaminingObject, ETriggerEvent::Triggered, this, &ThisClass::Input_StopExaminingObject);
	Ashley_InputComponent->BindNativeInputAction(InputConfigDataAsset, AshleyGameplayTags::Ashley_InputAction_RotateShowcase, ETriggerEvent::Triggered, this, &ThisClass::Input_RotateShowcase);
	Ashley_InputComponent->BindNativeInputAction(InputConfigDataAsset, AshleyGameplayTags::Ashley_InputAction_Esc, ETriggerEvent::Triggered, this, &ThisClass::Input_Esc);
	Ashley_InputComponent->BindNativeInputAction(InputConfigDataAsset, AshleyGameplayTags::Ashley_InputAction_Inventory, ETriggerEvent::Triggered, this, &ThisClass::Input_Inventory);
	Ashley_InputComponent->BindNativeInputAction(InputConfigDataAsset, AshleyGameplayTags::Ashley_InputAction_SkipDialogue, ETriggerEvent::Triggered, this, &ThisClass::Input_SkipDialogue);

	// Puzzle
	Ashley_InputComponent->BindNativeInputAction(InputConfigDataAsset, AshleyGameplayTags::Ashley_InputAction_ExitPuzzle, ETriggerEvent::Triggered, this, &ThisClass::Input_ExitPuzzle);
	
	// Cutscene
	Ashley_InputComponent->BindNativeInputAction(InputConfigDataAsset, AshleyGameplayTags::Ashley_InputAction_SkipCutscene, ETriggerEvent::Started, this, &ThisClass::Input_SkipCutscene_Started);
	Ashley_InputComponent->BindNativeInputAction(InputConfigDataAsset, AshleyGameplayTags::Ashley_InputAction_SkipCutscene, ETriggerEvent::Ongoing, this, &ThisClass::Input_SkipCutscene_Ongoing);
	Ashley_InputComponent->BindNativeInputAction(InputConfigDataAsset, AshleyGameplayTags::Ashley_InputAction_SkipCutscene, ETriggerEvent::Canceled, this, &ThisClass::Input_SkipCutscene_Canceled);
	Ashley_InputComponent->BindNativeInputAction(InputConfigDataAsset, AshleyGameplayTags::Ashley_InputAction_SkipCutscene, ETriggerEvent::Completed, this, &ThisClass::Input_SkipCutscene_Completed);

}

void AAshley_PlayerController::Input_Move(const FInputActionValue& InputActionValue)
{
	if (bInputBlocked) 
		return;

	if (ControlledPawn)
	{
		const FVector2D MovementVector = InputActionValue.Get<FVector2D>();

		// When we change directions, we update the coordinate system
		if (MovementVector != LastMovementVector)
		{
			CurrentForward = CameraManager->GetCurrentForward();
			CurrentRight = CameraManager->GetCurrentRight();
		}

		if (MovementVector.Y != 0.f)
		{
			ControlledPawn->AddMovementInput(CurrentForward, MovementVector.Y);
		} 

		if (MovementVector.X != 0.f)
		{
			ControlledPawn->AddMovementInput(CurrentRight, MovementVector.X);
		}

		LastMovementVector = MovementVector;
	}
	else ControlledPawn = GetPawn();
}

void AAshley_PlayerController::Input_Jump()
{
	if (bInputBlocked) return;

	if (MyCharacter) MyCharacter->Jump();

	else ControlledPawn = GetPawn();
}

void AAshley_PlayerController::Input_Esc()
{
	OnInputClicked.Broadcast(AshleyGameplayTags::Ashley_InputAction_Esc);

	UGameplayStatics::GetGameInstance(this)->GetSubsystem<UAshley_WidgetSystem>()->TogglePauseMenu();
}

void AAshley_PlayerController::Input_Inventory()
{
	if (bInputBlocked) return;

	OnInputClicked.Broadcast(AshleyGameplayTags::Ashley_InputAction_Inventory);

	UGameplayStatics::GetGameInstance(this)->GetSubsystem<UAshley_WidgetSystem>()->ToggleInventory();
}

void AAshley_PlayerController::Input_Sprint_Start(const FInputActionValue& InputActionValue)
{
	if (!IsValid(MyCharacter)) return;

	if (bIsSprintBlocked) return;
	
	bIsSpringting = true;
	MyCharacter->Sprint(true);
	MyCharacter->DropItem();
}

void AAshley_PlayerController::Input_Sprint_Stop(const FInputActionValue& InputActionValue)
{
	if (!IsValid(MyCharacter)) return;

	if (bIsSprintBlocked) return;

	bIsSpringting = false;
	MyCharacter->Sprint(false);
}

void AAshley_PlayerController::Input_SkipDialogue()
{
	Debug::Print("Clicked skipdialogue");

	//OnInputClicked.Broadcast(AshleyGameplayTags::Ashley_InputAction_Inventory);

	UAshley_WidgetSystem* WidgetSystem = UGameplayStatics::GetGameInstance(this)->GetSubsystem<UAshley_WidgetSystem>();

	// Se la battuta si sta ancora scrivendo, la prima pressione la completa senza passare alla successiva
	if (WidgetSystem->TryCompleteTypewriter()) return;

	// TODO: rimuovere, delegare tutto a dialogueComponent
	WidgetSystem->NextDialogueLine(true);

	auto DialogueComponent = MyCharacter->GetComponentByClass<UAshley_DialogueComponent>();
	if (DialogueComponent)
		DialogueComponent->SkipToNextLine();
}

void AAshley_PlayerController::Input_DropItem()
{
	Debug::Print("Clicked drop item");

	if (!IsValid(MyCharacter)) return;

	MyCharacter->DropItem();
}

void AAshley_PlayerController::TryMouseInteract()
{
	FHitResult HitResult;

	// Perform a raycast from the mouse cursor into the world.
	const bool bHit = GetHitResultUnderCursorByChannel(
		UEngineTypes::ConvertToTraceType(ECC_Visibility),
		true,
		HitResult
	);

	if (!bHit)
	{
		UE_LOG(LogTemp, Log, TEXT("Nothing clicked"));
		return;
	}

	// Get the Actor that was hit.
	AActor* HitActor = HitResult.GetActor();

	if (!HitActor)
	{
		return;
	}

	UE_LOG(
		LogTemp,
		Log,
		TEXT("Clicked Actor: %s"),
		*HitActor->GetName()
	);

	if (HitActor->Implements<UAshley_ClickableInterface>())
	{
		IAshley_ClickableInterface::Execute_Click(HitActor);
	}
}

void AAshley_PlayerController::Input_Interact()
{
	if (!IsValid(MyCharacter)) return;

	// Se c'e' un dialogo in corso, E salta la battuta invece di interagire
	UAshley_WidgetSystem* WidgetSystem = UGameplayStatics::GetGameInstance(this)->GetSubsystem<UAshley_WidgetSystem>();
	if (WidgetSystem && WidgetSystem->IsDialogueActive())
	{
		Input_SkipDialogue();
		return;
	}

	if (bInputBlocked) return;

	MyCharacter->Interact();
}

// *** INPUT Cutscenes ***

void AAshley_PlayerController::Input_SkipCutscene_Started(const FInputActionValue& InputActionValue)
{
	PressStartTime = GetWorld()->GetTimeSeconds();

	// Show skip cutscene widget
	UGameplayStatics::GetGameInstance(this)->GetSubsystem<UAshley_WidgetSystem>()->ShowSkipCutscenePrompt(true);
}

void AAshley_PlayerController::Input_SkipCutscene_Ongoing(const FInputActionValue& InputActionValue)
{
	float HeldTime = GetWorld()->GetTimeSeconds() - PressStartTime;
	float Percent = HeldTime / SequenceSkipTimer;

	// Update Percent on skip cutscene widget
	UGameplayStatics::GetGameInstance(this)->GetSubsystem<UAshley_WidgetSystem>()->UpdateSkipCutscenePrompt(Percent);
}

void AAshley_PlayerController::Input_SkipCutscene_Canceled(const FInputActionValue& InputActionValue)
{
	// Hide skip cutscene widget
	UGameplayStatics::GetGameInstance(this)->GetSubsystem<UAshley_WidgetSystem>()->ShowSkipCutscenePrompt(false);
}

void AAshley_PlayerController::Input_SkipCutscene_Completed(const FInputActionValue& InputActionValue)
{
	// Hide skip cutscene widget
	UGameplayStatics::GetGameInstance(this)->GetSubsystem<UAshley_WidgetSystem>()->ShowSkipCutscenePrompt(false);
	AAshley_SequenceManager* SeqManager = (AAshley_SequenceManager*) UGameplayStatics::GetActorOfClass(GetWorld(), AAshley_SequenceManager::StaticClass());
	SeqManager->SkipSequence();
}

void AAshley_PlayerController::Input_Mouse_Pressed(const FInputActionValue& InputActionValue)
{
	bMousePressed = true;

	// Il click funziona anche in pausa (es. showcase), ma non deve attraversare i menu aperti
	if (UGameplayStatics::GetGameInstance(this)->GetSubsystem<UAshley_WidgetSystem>()->IsAnyMenuOpen()) return;

	// TODO: mandare evento ?
	TryMouseInteract();
}

void AAshley_PlayerController::Input_Mouse_Released(const FInputActionValue& InputActionValue)
{
	bMousePressed = false;
	Debug::Print("release");
	// TODO: mandare evento ?
}

// *** INPUT Puzzle ***

void AAshley_PlayerController::Input_ExitPuzzle()
{
	Debug::Print("EXIT PUZZLE");
}

void AAshley_PlayerController::Input_StopExaminingObject()
{
	if (!IsValid(MyCharacter)) return;

	MyCharacter->StopExaminingObject();
}

void AAshley_PlayerController::Input_RotateShowcase(const FInputActionValue& InputActionValue)
{
	if (!IsValid(MyCharacter)) return;

	MyCharacter->RotateShowcaseItem(InputActionValue.Get<FVector2D>());
}

UAshley_CameraManager* AAshley_PlayerController::GetCameraManager()
{
	// Created as default subobject in the constructor: always valid while the controller exists
	return CameraManager;
}

void AAshley_PlayerController::ToggleInputBlock(bool bShouldBlockInput)
{
	bInputBlocked = bShouldBlockInput;
}

void AAshley_PlayerController::ToggleSprintBlock(bool bShouldBlockSprint)
{
	bIsSprintBlocked = bShouldBlockSprint;
}

void AAshley_PlayerController::ToggleHidePlayer(bool bShouldHidePlayer)
{
	if (ControlledPawn)
	{
		MyCharacter = Cast<AAshley_CharacterPlayer>(ControlledPawn);
		if (MyCharacter)
		{
			MyCharacter->ToggleActorActive(!bShouldHidePlayer);
			MyCharacter->HideObjectsInHand(bShouldHidePlayer);
		}
	}
}

void AAshley_PlayerController::TriggerPlayerReset()
{
	if (!CurrentCheckpoint) return;
	// Setto posizione dell'ultimo checkpoint
	MyCharacter->SetActorLocationAndRotation(CurrentCheckpoint->Location, CurrentCheckpoint->Rotation);

	// contiene location, oggetti fino al scorso reset?

	// OnPlayerDeathReset();
}

void AAshley_PlayerController::RegisterCheckpoint(UCheckpointData* SaveData)
{
	CurrentCheckpoint = SaveData;
}

void AAshley_PlayerController::SetMappingContextByName(EAshley_InputMappingContext MappingContextName)
{
	//Debug::Print("SETTING MAPPING");

	if (ULocalPlayer* LocalPlayer = GetLocalPlayer())
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer))
		{
			Subsystem->AddMappingContext(InputConfigDataAsset->GetMappingContextByName(MappingContextName), 0);
			Subsystem->RemoveMappingContext(CurrentInputMapping);
			CurrentInputMapping = InputConfigDataAsset->GetMappingContextByName(MappingContextName);
		}
	}
}

void AAshley_PlayerController::AddMappingContextByName(EAshley_InputMappingContext MappingContextName, int Priority)
{
	if (ULocalPlayer* LocalPlayer = GetLocalPlayer())
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer))
		{
			Subsystem->AddMappingContext(InputConfigDataAsset->GetMappingContextByName(MappingContextName), Priority);
		}
	}
}

void AAshley_PlayerController::RemoveMappingContextByName(EAshley_InputMappingContext MappingContextName)
{
	if (ULocalPlayer* LocalPlayer = GetLocalPlayer())
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer))
		{
			Subsystem->RemoveMappingContext(InputConfigDataAsset->GetMappingContextByName(MappingContextName));
		}
	}
}
