// Ashley


#include "Episode01/Ashley_Doorman.h"

#include "Blueprint/AIAsyncTaskBlueprintProxy.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Perception/AIPerceptionComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

#include "Characters/Players/Ashley_CharacterPlayer.h"
#include "Components/Ashley_InventoryComponent.h"
#include "Core/Ashley_PlayerController.h"
#include "Core/Ashley_TriggerCollider.h"
#include "DataAssets/Ashley_GenericDialogue.h"
#include "Episode01/Ashley_ApartmentLevelManager.h"
#include "Interaction/Ashley_Door.h"
#include "Managers/Ashley_EventManagerSystem.h"
#include "Tags_Enums_Structs/Ashley_GameplayTags.h"
#include "MyBlueprintFunctionLibrary.h"

#include "DebugHelper.h"


AAshley_Doorman::AAshley_Doorman()
{
}

void AAshley_Doorman::BeginPlay()
{
	Super::BeginPlay();

	bIsSitting = true;

	if (Trigger1)
	{
		Trigger1->SetActive(false);
		Trigger1->OnColliderTrigger.AddDynamic(this, &AAshley_Doorman::OnBoxDialogueHit);
	}

	if (UAshley_EventManagerSystem* EventManager = GetGameInstance()->GetSubsystem<UAshley_EventManagerSystem>())
	{
		EventManager->OnEventTriggered.AddDynamic(this, &AAshley_Doorman::OnPlayerEvent);
	}

	PerceptionComponent = FindComponentByClass<UAIPerceptionComponent>();
	if (PerceptionComponent)
	{
		PerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &AAshley_Doorman::OnTargetPerceptionUpdated);
	}
}

// ---------------------------------------------------------------------------
// Dialogo
// ---------------------------------------------------------------------------

void AAshley_Doorman::Interact_Implementation()
{
	PlayIntroDialogue();
}

void AAshley_Doorman::OnBoxDialogueHit()
{
	if (bIsDoormanWatching && !bIsTalking)
	{
		PlayIntroDialogue();
	}
}

void AAshley_Doorman::PlayIntroDialogue()
{
	if (bIntroDialoguePlayed)
	{
		StartDialogue(IntroShortDialogue);
	}
	else
	{
		StartDialogue(IntroDialogue);
		bIntroDialoguePlayed = true;
	}
}

void AAshley_Doorman::EndDialogue()
{
	Super::EndDialogue();

	if (FinalDialogue)
	{
		FinalDialogue->PlayDialogue(GetGameInstance(), 0);
	}

	if (LevelManager)
	{
		LevelManager->SetFirstDialogueDone();
	}

	if (AAshley_CharacterPlayer* Player = GetPlayerCharacter())
	{
		if (UAshley_InventoryComponent* Inventory = Player->GetInventory())
		{
			Inventory->PutObjectInventory(PhotoItemID, EPickupType::COLLECTABLE, 1);
		}
	}
}

// ---------------------------------------------------------------------------
// Movimento verso Quadro Elettrico
// ---------------------------------------------------------------------------

void AAshley_Doorman::GoToCheckElectricalPanel_Implementation()
{
	if (Trigger1)
	{
		Trigger1->SetActive(false);
	}

	GetWorldTimerManager().SetTimer(ElectricalPanelTimer, this, &AAshley_Doorman::OnElectricalPanelDelayElapsed, ElectricalPanelDelay, false);
}

void AAshley_Doorman::OnElectricalPanelDelayElapsed()
{
	if (ConciergeDoor)
	{
		ConciergeDoor->SwitchToOpen();
	}

	bIsDoormanWatching = false;
	bIsSitting = false;
	bIsTalking = false;

	if (UAIAsyncTaskBlueprintProxy* Proxy = MoveToActor(CourtyardDoor, DefaultAcceptanceRadius, false))
	{
		Proxy->OnSuccess.AddDynamic(this, &AAshley_Doorman::OnReachedCourtyardDoor);
		Proxy->OnFail.AddDynamic(this, &AAshley_Doorman::OnReachedCourtyardDoor);
	}
}

void AAshley_Doorman::OnReachedCourtyardDoor(EPathFollowingResult::Type MovementResult)
{
	if (CourtyardDoor)
	{
		CourtyardDoor->SwitchToOpen();
	}

	if (!bPlayerSpotted)
	{
		MoveToActor(ElectricalPanelDestination, DefaultAcceptanceRadius, false);
	}
}

// ---------------------------------------------------------------------------
// Movimento verso Intercom
// ---------------------------------------------------------------------------

void AAshley_Doorman::GoCheckIntercom()
{
	bIsDoormanWatching = false;
	bIsSitting = false;
	bIsTalking = false;

	GetWorldTimerManager().SetTimer(IntercomTimer, this, &AAshley_Doorman::OnIntercomDelayElapsed, IntercomDelay, false);
}

void AAshley_Doorman::OnIntercomDelayElapsed()
{
	if (bIsDoormanWatching)
	{
		return;
	}

	if (Trigger1)
	{
		Trigger1->SetActive(false);
	}

	if (ConciergeDoor)
	{
		ConciergeDoor->SwitchToOpen();
	}

	if (UAIAsyncTaskBlueprintProxy* Proxy = MoveToActor(IntercomDestination, IntercomAcceptanceRadius, true))
	{
		Proxy->OnSuccess.AddDynamic(this, &AAshley_Doorman::OnReachedIntercom);
		Proxy->OnFail.AddDynamic(this, &AAshley_Doorman::OnReachedIntercom);
	}
}

void AAshley_Doorman::OnReachedIntercom(EPathFollowingResult::Type MovementResult)
{
	if (!IntercomDestination)
	{
		return;
	}

	FVector TargetLocation = IntercomDestination->GetActorLocation();
	if (const UStaticMeshComponent* IntercomMesh = IntercomDestination->FindComponentByClass<UStaticMeshComponent>())
	{
		TargetLocation = IntercomMesh->GetComponentLocation();
	}

	const FRotator LookAt = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), TargetLocation);
	SetActorRotation(FRotator(0.f, LookAt.Yaw, 0.f));
}

// ---------------------------------------------------------------------------
// Perception
// ---------------------------------------------------------------------------

void AAshley_Doorman::OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	if (bIsDoormanWatching)
	{
		return;
	}

	if (!Actor || Actor != GetPlayerCharacter())
	{
		return;
	}

#if !UE_BUILD_SHIPPING
	Debug::Print(TEXT("ENEMY SPOTTED!"));
#endif

	bPlayerSpotted = true;
	GetCharacterMovement()->StopActiveMovement();

	if (LevelManager)
	{
		LevelManager->ResetLevel();
	}
}

void AAshley_Doorman::TriggerPlayerDeath()
{
	if (AAshley_PlayerController* PlayerController = GetPlayerController())
	{
		PlayerController->ToggleInputBlock(true);
	}
}

// ---------------------------------------------------------------------------
// Riferimenti al giocatore (cache lazy: il player potrebbe non essere ancora posseduto al BeginPlay)
// ---------------------------------------------------------------------------

AAshley_CharacterPlayer* AAshley_Doorman::GetPlayerCharacter()
{
	if (!CachedPlayerCharacter)
	{
		CachedPlayerCharacter = Cast<AAshley_CharacterPlayer>(UGameplayStatics::GetPlayerCharacter(this, 0));
	}
	return CachedPlayerCharacter;
}

AAshley_PlayerController* AAshley_Doorman::GetPlayerController()
{
	if (!CachedPlayerController)
	{
		CachedPlayerController = Cast<AAshley_PlayerController>(UGameplayStatics::GetPlayerController(this, 0));
	}
	return CachedPlayerController;
}

// ---------------------------------------------------------------------------
// Eventi cutscene
// ---------------------------------------------------------------------------

void AAshley_Doorman::OnPlayerEvent(FGameplayTag EventName, FString EventParameter)
{
	if (EventName == AshleyGameplayTags::Ashley_Events_PlaySequence)
	{
		GetMesh()->SetVisibility(false, true);
		return;
	}

	if (EventName != AshleyGameplayTags::Ashley_Events_EndSequence)
	{
		return;
	}

	GetMesh()->SetVisibility(true, true);

	FGameplayTag SequenceTag;
	const bool bTagFound = UMyBlueprintFunctionLibrary::TryRequestGameplayTag(FName(*EventParameter), SequenceTag);
	if (!bTagFound || SequenceTag != DoormanCutsceneName)
	{
		return;
	}

	// Riposiziono Player e Doorman nella posizione corretta dopo la cutscene
	if (SpawnTransform)
	{
		SetActorLocationAndRotation(SpawnTransform->GetActorLocation(), SpawnTransform->GetActorRotation());
	}

	if (AAshley_CharacterPlayer* Player = GetPlayerCharacter())
	{
		if (CharacterSpawnTransform)
		{
			Player->SetActorLocationAndRotation(CharacterSpawnTransform->GetActorLocation(), CharacterSpawnTransform->GetActorRotation());
		}
	}

	GetWorldTimerManager().SetTimer(CutsceneDialogueTimer, this, &AAshley_Doorman::OnCutsceneDialogueDelayElapsed, CutsceneDialogueDelay, false);
}

void AAshley_Doorman::OnCutsceneDialogueDelayElapsed()
{
	StartDialogue(IntroShortDialogue);
	bIntroDialoguePlayed = true;

	if (Trigger1)
	{
		Trigger1->SetActive(true);
	}
}

// ---------------------------------------------------------------------------
// Reset
// ---------------------------------------------------------------------------

void AAshley_Doorman::ResetToInitialState_Implementation()
{
#if !UE_BUILD_SHIPPING
	Debug::Print(TEXT("Resetting doorman"));
#endif

	GetWorldTimerManager().ClearTimer(ElectricalPanelTimer);
	GetWorldTimerManager().ClearTimer(IntercomTimer);
	GetWorldTimerManager().ClearTimer(CutsceneDialogueTimer);

	bIsDoormanWatching = true;
	bPlayerSpotted = false;
	bIsTalking = false;
	bIsSitting = true;

	GetCharacterMovement()->StopMovementImmediately();

	if (SpawnTransform)
	{
		SetActorLocationAndRotation(SpawnTransform->GetActorLocation(), SpawnTransform->GetActorRotation());
	}

	if (ConciergeDoor)
	{
		ConciergeDoor->ResetState();
	}

	if (CourtyardDoor)
	{
		CourtyardDoor->ResetState();
	}

	if (Trigger1)
	{
		Trigger1->SetActive(true);
	}
}

// ---------------------------------------------------------------------------
// Helpers
// ---------------------------------------------------------------------------

UAIAsyncTaskBlueprintProxy* AAshley_Doorman::MoveToActor(AActor* Target, float AcceptanceRadius, bool bStopOnOverlap)
{
	if (!Target)
	{
		return nullptr;
	}

	return UAIBlueprintHelperLibrary::CreateMoveToProxyObject(this, this, FVector::ZeroVector, Target, AcceptanceRadius, bStopOnOverlap);
}
